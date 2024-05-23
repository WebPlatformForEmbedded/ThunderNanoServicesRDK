/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
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

#include "CookieJar.h"

#include <sstream>
#include <iterator>
#include <algorithm>

#include <glib.h>
#include <zlib.h>

#include <cryptography/cryptography.h>


namespace WPEFramework {
namespace Plugin {

namespace {

static constexpr uint8_t PAYLOAD_VERSION = 1;

static std::string serialize(const std::vector<std::string>& cookies)
{
    std::ostringstream os;
    std::copy(cookies.begin(), cookies.end(), std::ostream_iterator<std::string>(os, "\n"));
    return os.str();
}

static void deserialize(const std::string& cookies, std::vector<std::string>& result)
{
    std::string cookie;
    std::stringstream ss(cookies);
    while (ss.good()) {
        getline(ss, cookie, '\n');
        if (!cookie.empty())
            result.push_back(cookie);
    }
}

static std::vector<uint8_t> compress(const std::string& str)
{
    std::vector<uint8_t> result;
    size_t nbytes = str.size();
    if (nbytes == 0) {
        result.resize(4, '\0');
        return result;
    }
    const int compressionLevel = 1;
    unsigned long len = nbytes + nbytes / 100 + 13;
    int status;
    do {
        result.resize(len + 4);
        status = ::compress2((unsigned char*)result.data() + 4, &len,
            (const unsigned char*)str.c_str(), nbytes, compressionLevel);
        switch (status)
        {
        case Z_OK:
            result.resize(len + 4);
            result[0] = (nbytes & 0xff000000) >> 24;
            result[1] = (nbytes & 0x00ff0000) >> 16;
            result[2] = (nbytes & 0x0000ff00) >> 8;
            result[3] = (nbytes & 0x000000ff);
            break;
        case Z_MEM_ERROR:
            TRACE_GLOBAL(Trace::Error,(_T("Z_MEM_ERROR: Not enough memory")));
            result.resize(0);
            break;
        case Z_BUF_ERROR:
            len *= 2;
            break;
        }
    }
    while (status == Z_BUF_ERROR);
    return result;
}

static std::string uncompress(const std::vector<uint8_t>& in)
{
    std::string result;
    size_t nbytes = in.size();
    if (nbytes <= 4) {
        if (nbytes < 4 || std::any_of(in.cbegin(), in.cend(), [](int v) {return v != 0;})) {
            TRACE_GLOBAL(Trace::Error,(_T("Input data is corrupted")));
        }
        return result;
    }
    const unsigned char* data = (const unsigned char*) in.data();
    unsigned long expectedSize = (unsigned long)(
        (data[0] << 24) | (data[1] << 16) |
        (data[2] <<  8) | (data[3]));
    unsigned long len = std::max(expectedSize, 1ul);
    int status;
    do {
        result.resize(len);
        status = ::uncompress((unsigned char*)result.data(), &len, data + 4, nbytes - 4);
        switch (status)
        {
        case Z_BUF_ERROR:
            len *= 2;
            break;
        case Z_MEM_ERROR:
            TRACE_GLOBAL(Trace::Error,(_T("Z_MEM_ERROR: Not enough memory")));
            result.resize(0);
            break;
        case Z_DATA_ERROR:
            TRACE_GLOBAL(Trace::Error,(_T("Z_DATA_ERROR: Input data is corrupted")));
            result.resize(0);
            break;
        }
    }
    while (status == Z_BUF_ERROR);
    return result;
}

// CRC-16 for compatability with rdkbrowser / rdkbrwoser2
static uint32_t crc_checksum(const std::string& str)
{
    static const unsigned short crc_tbl[16] = {
        0x0000, 0x1081, 0x2102, 0x3183,
        0x4204, 0x5285, 0x6306, 0x7387,
        0x8408, 0x9489, 0xa50a, 0xb58b,
        0xc60c, 0xd68d, 0xe70e, 0xf78f
    };
    unsigned short crc = 0xffff;
    unsigned char c = 0;
    const unsigned char *p = (const unsigned char*) str.data();
    size_t len = str.size();
    while (len--) {
        c = *p++;
        crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
        c >>= 4;
        crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
    }
    return ~crc & 0xffff;
}

} // namespace

class CookieJar::CookieJarPrivate
{
public:
    CookieJarPrivate()
        : _lock()
        , _connector()
        , _key()
    {
    }
    ~CookieJarPrivate() = default;

    CookieJarPrivate(const CookieJarPrivate&) = delete;
    CookieJarPrivate& operator=(const CookieJarPrivate&) = delete;

public:
    uint32_t Configure(const string& connector, const string& key)
    {
        uint32_t rc = Core::ERROR_NONE;

        _lock.Lock();

        if (key.empty() == false) {
            ASSERT(connector.empty() == false);

            if ((_key != key) || (_connector != connector))  {

                _connector = connector;
                _key = key;

                TRACE_GLOBAL(Trace::Information, (_T("CookieJar crypto configured (%s)"), _connector.c_str()));
            }
        }
        else {
            ASSERT(connector.empty() == true);

            _connector.clear();
            _key.clear();

            TRACE_GLOBAL(Trace::Information, (_T("CookieJar crypto unconfigured")));
        }

        _lock.Unlock();

        return (rc);
    }

public:
    uint32_t Pack(const std::vector<std::string> &cookies, uint32_t& version, uint32_t& checksum, string& payload) const
    {
        uint32_t rc = Core::ERROR_GENERAL;

        payload.clear();

        const string serialized = serialize(cookies);

        if (serialized.size() == 0) {
            rc = Core::ERROR_NONE;
        }
        else {
            const std::vector<uint8_t> compressed = compress(serialized);
            ASSERT(compressed.size() != 0);

            if (compressed.size() == 0) {
                TRACE_GLOBAL(Trace::Error, (_T("Compression failed")));
            }
            else {
                uint32_t encryptedSize = (compressed.size() + 32 /* some headroom for padding and IV */);
                uint8_t* const encrypted = new (std::nothrow) uint8_t[encryptedSize];
                ASSERT(encrypted != nullptr);

                encryptedSize = Cipher(true, compressed.size(), compressed.data(), encryptedSize, encrypted);

                if (encryptedSize == 0) {
                    TRACE_GLOBAL(Trace::Error,(_T("Encryption failed")));
                }
                else {
                    Core::ToString(encrypted, encryptedSize, true, payload);
                    ASSERT(payload.empty() == false);

                    if (payload.empty() == true) {
                        TRACE_GLOBAL(Trace::Error,(_T("Encoding failed")));
                    }
                    else {
                        version = PAYLOAD_VERSION;
                        checksum = crc_checksum(serialized);
                        rc = Core::ERROR_NONE;
                    }
                }

                delete[] encrypted;
            }
        }

        return rc;
    }

    uint32_t Unpack(const uint32_t version, const uint32_t checksum, const string& payload, std::vector<std::string>& cookies) const
    {
        uint32_t rc = Core::ERROR_GENERAL;

        cookies.clear();

        if (payload.size() == 0) {
            rc = Core::ERROR_NONE;
        }
        else if (version != PAYLOAD_VERSION) {
            TRACE_GLOBAL(Trace::Error,(_T("Unsupported payload version (%d)"), version));
        }
        else {
            uint32_t unencodedSize = ((payload.size() * 3) / 4); // base64
            uint8_t* const unencoded = new (std::nothrow) uint8_t[unencodedSize];
            ASSERT(unencoded != nullptr);

            const uint32_t decoded = Core::FromString(payload, unencoded, unencodedSize);

            if ((unencodedSize == 0) || (decoded != payload.size())) {
                TRACE_GLOBAL(Trace::Error,(_T("Decoding failed")));
            }
            else {
                std::vector<uint8_t> decrypted(unencodedSize + 32);

                const uint32_t decryptedSize = Cipher(false, unencodedSize, unencoded, decrypted.size(), decrypted.data());
                decrypted.resize(decryptedSize); // does not realloc!

                if (decryptedSize == 0) {
                    TRACE_GLOBAL(Trace::Error,(_T("Decryption failed")));
                }
                else {
                    const string serialized = uncompress(decrypted);

                    if (serialized.size() == 0) {
                        TRACE_GLOBAL(Trace::Error,(_T("Decompression failed")));
                    }
                    else {
                        const uint32_t actualChecksum = crc_checksum(serialized);

                        if (actualChecksum != checksum) {
                            TRACE_GLOBAL(Trace::Error,(_T("Checksum does not match: actual=%04x expected=%04x"), actualChecksum, checksum));
                        }
                        else {
                            deserialize(serialized, cookies);
                            rc = Core::ERROR_NONE;
                        }
                    }
                }
            }

            delete[] unencoded;
        }

        return rc;
    }

private:
    uint32_t Cipher(const bool encrypt, const uint32_t inputSize, const uint8_t input[], const uint32_t outputSize, uint8_t output[]) const
    {
        uint32_t rc = 0;

        ASSERT(inputSize != 0);
        ASSERT(input != nullptr);
        ASSERT(outputSize != 0);
        ASSERT(output != nullptr);

        _lock.Lock();

        uint32_t id = 0;
        Exchange::IVault* vault = nullptr;
        Exchange::ICryptography* crypto = nullptr;

        if (_connector.empty() == false) {
            // Don't allow local cryptography instance.
            crypto = Exchange::ICryptography::Instance(_connector);
        }

        if (crypto == nullptr) {
            TRACE_GLOBAL(Trace::Error, (_T("Cryptography is not available")));
        }
        else {
            Exchange::IDeviceObjects* const objects = Exchange::IDeviceObjects::Instance();
            if (objects != nullptr) {
                id = objects->Id(_key, vault);

                if (vault == nullptr) {
                    ASSERT(id == 0);
                    TRACE_GLOBAL(Trace::Error, (_T("CookieJar encryption key '%s' not provisioned"), _key.c_str()));
                }

                objects->Release();
            }
            else {
                TRACE_GLOBAL(Trace::Error, (_T("Svalbard not available")));
            }
        }

        if (vault != nullptr) {
            ASSERT(id != 0);

            Exchange::ICipher* const cipher = vault->AES(Exchange::aesmode::CBC, id);

            if (cipher == nullptr) {
                TRACE_GLOBAL(Trace::Error, (_T("Cipher failed (unsupported method)")));
            }
            else {
                static constexpr uint16_t IV_SIZE = 16;

                const uint8_t* iv = nullptr;
                const uint8_t* inputBuffer = nullptr;
                uint32_t inputBufferSize = 0;
                uint8_t* outputBuffer = nullptr;
                uint32_t outputBufferSize = 0;

                if (encrypt == true) {
                    if (outputSize >= IV_SIZE) {
                        Exchange::IRandom* const random = crypto->Random();
                        ASSERT(random != nullptr);

                        if (random != nullptr) {
                            uint8_t* newIV = output;

                            if (random->Generate(IV_SIZE, newIV) == IV_SIZE) {
                                iv = newIV;
                                inputBuffer = input;
                                inputBufferSize = inputSize;
                                outputBuffer = (output + IV_SIZE);
                                outputBufferSize = (outputSize - IV_SIZE);
                            }

                            random->Release();
                        }
                    }
                }
                else if (inputSize >= IV_SIZE) {
                    iv = input;
                    inputBuffer = (input + IV_SIZE);
                    inputBufferSize = (inputSize - IV_SIZE);
                    outputBuffer = output;
                    outputBufferSize = outputSize;
                }

                if (inputBufferSize == 0) {
                    TRACE_GLOBAL(Trace::Error, (_T("Cipher failed (invalid input)")));
                }
                else {
                    ASSERT(iv != nullptr);
                    ASSERT(inputBuffer != nullptr);
                    ASSERT(outputBuffer != nullptr);

                    if (encrypt == true) {
                        rc = cipher->Encrypt(IV_SIZE, iv, inputBufferSize, inputBuffer, outputBufferSize, outputBuffer);

                        if (rc != 0) {
                            rc += IV_SIZE;
                        }
                    }
                    else {
                        rc = cipher->Decrypt(IV_SIZE, iv, inputBufferSize, inputBuffer, outputBufferSize, outputBuffer);
                    }

                    if (rc == 0) {
                        TRACE_GLOBAL(Trace::Error, (_T("Cipher failed")));
                    }
                }

                cipher->Release();
            }

            vault->Release();
        }

        if (crypto != nullptr) {
            crypto->Release();
        }

        _lock.Unlock();

        return (rc);
    }

private:
    mutable Core::CriticalSection _lock;
    string _connector;
    string _key;
};

CookieJar::CookieJar()
    : _priv()
{
    _priv = std::unique_ptr<CookieJar::CookieJarPrivate>(new CookieJar::CookieJarPrivate());
}

CookieJar::~CookieJar()
{
}

uint32_t CookieJar::Configure(const string& connector, const string& key)
{
    ASSERT(_priv != nullptr);
    return (_priv->Configure(connector, key));
}

uint32_t CookieJar::Pack(uint32_t& version, uint32_t& checksum, string& payload) const
{
    ASSERT(_priv != nullptr);
    return _priv->Pack(_cookies, version, checksum, payload);
}

uint32_t CookieJar::Unpack(const uint32_t version, const uint32_t checksum, const string& payload)
{
    uint32_t rc;
    std::vector<std::string> cookies;

    ASSERT(_priv != nullptr);
    rc = _priv->Unpack(version, checksum, payload, cookies);

    if (rc == WPEFramework::Core::ERROR_NONE) {
        _cookies = std::move(cookies);
        _refreshed.SetState( false );
    }

    return rc;
}

void CookieJar::SetCookies(std::vector<std::string> && cookies)
{
    _cookies = std::move(cookies);
    _refreshed.SetState( true );
}

std::vector<std::string> CookieJar::GetCookies() const
{
    return _cookies;
}

} // namespace Plugin
}
