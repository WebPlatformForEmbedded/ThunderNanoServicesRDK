/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2023 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../../Module.h"

#include <interfaces/ISimpleCrypto.h>
#include <interfaces/IConfiguration.h>
#include <interfaces/ICryptography.h>

#ifdef USE_OPENSSL
#include <openssl/rand.h>
#else
#include <cryptalgo/cryptalgo.h>
#endif

namespace WPEFramework {
namespace Plugin {

    class SimpleCryptoImplementation : public Exchange::ISimpleCrypto
                                     , public Exchange::IConfiguration {
    private:
        static constexpr uint32_t MAX_KEY_FILE_SIZE = 64;

    public:
        SimpleCryptoImplementation()
            : _lock()
            , _vault(nullptr)
            , _persistentPath()
            , _keyIdCache()
        {
#ifndef USE_OPENSSL
            Crypto::Reseed();
#endif
        }

        ~SimpleCryptoImplementation() override
        {
            if (_vault != nullptr) {
                _vault->Release();
            }
        }

        SimpleCryptoImplementation(const SimpleCryptoImplementation&) = delete;
        SimpleCryptoImplementation& operator= (const SimpleCryptoImplementation&) = delete;

    public:
        class AESCBCImplementation : public Exchange::ISimpleCrypto::ICipher {
        private:
            static constexpr uint8_t IV_SIZE = 16;
            static constexpr uint32_t MAX_INPUT_BUFFER_SIZE = 0x100000; // Enforce a 1 MB cap

        public:
            AESCBCImplementation(Exchange::ICipher* aes)
               : _aes(aes)
            {
                ASSERT(_aes != nullptr);
                _aes->AddRef();
            }
            ~AESCBCImplementation()
            {
                _aes->Release();
            }

        public:
            // Exchange::ISimpleCrypto::ICipher interface
            uint32_t Encrypt(const uint32_t length, const uint8_t input[], const uint32_t maxOutLength, uint8_t output[]) const override
            {
                int32_t result = 0;

                if ((length <= MAX_INPUT_BUFFER_SIZE) && (maxOutLength > IV_SIZE)) {
                    uint8_t iv[IV_SIZE];

#ifdef USE_OPENSSL
                    RAND_bytes(iv, IV_SIZE);
#else
                    for (uint8_t i = 0; i < IV_SIZE; i++) {
                        Crypto::Random(iv[i]); // CSPRNG?
                    }
#endif

                    // Encrypt the IV and the data
                    ::memcpy(output, iv, IV_SIZE);

                    result = _aes->Encrypt(IV_SIZE, iv, length, input, (maxOutLength - IV_SIZE), (output + IV_SIZE));

                    if (result < 0) {
                        result = 0;
                    }
                    else {
                        result += IV_SIZE;
                    }
                }

                return (result);
            }
            uint32_t Decrypt(const uint32_t length, const uint8_t input[], const uint32_t maxOutLength, uint8_t output[]) const override
            {
                int32_t result = 0;

                if ((length <= MAX_INPUT_BUFFER_SIZE) && (length >= IV_SIZE)) {

                    result = _aes->Decrypt(IV_SIZE, input, (length - IV_SIZE), (input + IV_SIZE), maxOutLength, output);

                    if (result < 0) {
                        result = 0;
                    }
                }

                return (result);
            }

        public:
            BEGIN_INTERFACE_MAP(CipherImplementation)
                INTERFACE_ENTRY(Exchange::ISimpleCrypto::ICipher)
            END_INTERFACE_MAP

        private:
            Exchange::ICipher* _aes;
        }; // class AESCBCImplementation

    public:
        // Exchange::ISimpleCrypto interface
        Exchange::ISimpleCrypto::ICipher* Cipher(const std::string& keyName) override
        {
            Exchange::ISimpleCrypto::ICipher* cipher = nullptr;

            const uint32_t keyId = KeyId(keyName);

            if ((keyId != 0) && (_vault != nullptr)) {
                Exchange::ICipher* aes = _vault->AES(Exchange::aesmode::CBC, keyId);
                ASSERT(aes != nullptr);

                if (aes != nullptr) {
                    cipher = Core::Service<AESCBCImplementation>::Create<Exchange::ISimpleCrypto::ICipher>(aes);
                    aes->Release();
                }
            }

            return (cipher);
        }

    public:
        // Exchange::IConfiguration interface
        uint32_t Configure(PluginHost::IShell* service) override
        {
            uint32_t result = Core::ERROR_NONE;

            Config config;
            config.FromString(service->ConfigLine());

            _lock.Lock();

            _persistentPath = service->PersistentPath();
            ASSERT(service->PersistentPath().empty() == false);

            if (config.Location.Value().empty() == false) {
                _persistentPath += config.Location.Value() + _T("/");

                bool valid = false;
                Core::File::Normalize(_persistentPath, valid);
                ASSERT(valid == true);
            }

            Core::Directory directory(_persistentPath.c_str());
            if (directory.CreatePath() != true) {
                TRACE(Trace::Error, (_T("Failed to create keys directory!")));
            }

            TRACE(Trace::Information, (_T("SimpleCrypto keys location: '%s'"), _persistentPath.c_str()));

            Exchange::ICryptography* crypto = Exchange::ICryptography::Instance(config.Connector.Value());
            ASSERT(crypto != nullptr);

            if (crypto != nullptr) {
                Exchange::CryptographyVault vaultId = Exchange::CRYPTOGRAPHY_VAULT_DEFAULT;

                if (config.Vault.Value() == _T("platform")) {
                    vaultId = Exchange::CRYPTOGRAPHY_VAULT_PLATFORM;
                }
                else if ((config.Vault.Value() != _T("defult")) && (config.Vault.Value().empty() == false)) {
                    TRACE(Trace::Error, (_T("Unsupported vault id, using default")));
                }

                _vault = crypto->Vault(vaultId);
                ASSERT(_vault != nullptr);

                if (_vault == nullptr) {
                    TRACE(Trace::Error, (_T("Vault %d unavailable!"), vaultId));
                }

                crypto->Release();
            }
            else {
                TRACE(Trace::Error, (_T("No ICryptography available!")));
                result = Core::ERROR_GENERAL;
            }

            _lock.Unlock();

            return (result);
        }

    public:
        BEGIN_INTERFACE_MAP(SimpleCryptoImplementation)
            INTERFACE_ENTRY(Exchange::ISimpleCrypto)
            INTERFACE_ENTRY(Exchange::IConfiguration)
        END_INTERFACE_MAP

    private:
        uint32_t KeyId(const string& name)
        {
            uint32_t id = 0;

            _lock.Lock();

            auto it = _keyIdCache.find(name);

            if (it != _keyIdCache.end()) {
                id = (*it).second;
            }
            else if (_vault != nullptr) {
                // First time use of the key, fetch it from disk.
                id = LoadKey(name);

                if (id != 0) {
                   _keyIdCache.emplace(name, id);
                }
            }

            _lock.Unlock();

            return (id);
        }
        uint32_t LoadKey(const string& name)
        {
            ASSERT(_vault != nullptr);

            uint32_t id = 0;

            Core::File keyFile(_persistentPath + name + _T(".key"));

            if ((keyFile.Exists() == true)) {

                if ((keyFile.Size() <= MAX_KEY_FILE_SIZE) && (keyFile.Open(true) == true)) {

                    const uint16_t size = static_cast<uint16_t>(keyFile.Size());

                    uint8_t* const key = static_cast<uint8_t*>(ALLOCA(size));
                    ASSERT(key != nullptr);

                    const uint32_t read = keyFile.Read(key, size);

                    if (read == size) {
                        id = _vault->Set(size, key);

                        TRACE(Trace::Information, (_T("Loaded named key '%s'"), name.c_str()));
                    }
                    else {
                        TRACE(Trace::Error, (_T("IO error while reading named key '%s'!"), name.c_str()));
                    }

                    keyFile.Close();
                }
            }
            else {
                TRACE(Trace::Error, (_T("Named key '%s' not found!"), name.c_str()));
            }

            return (id);
        }

    private:
        class Config : public Core::JSON::Container {
        public:
            Config()
                : Core::JSON::Container()
                , Connector()
                , Vault(_T("platform"))
                , Location(_T("keys"))
            {
                Add(_T("connector"), &Connector);
                Add(_T("vault"), &Vault);
                Add(_T("location"), &Location);
            }

        public:
            Core::JSON::String Connector;
            Core::JSON::String Vault;
            Core::JSON::String Location;
        }; // class Config

    private:
        Core::CriticalSection _lock;
        Exchange::IVault* _vault;
        string _persistentPath;
        std::map<string, uint32_t> _keyIdCache;
    };

    SERVICE_REGISTRATION(SimpleCryptoImplementation, 1, 0)

} // namespace Plugin
}
