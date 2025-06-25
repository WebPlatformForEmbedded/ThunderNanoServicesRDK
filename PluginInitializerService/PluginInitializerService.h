/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2025 [PLEASE ADD COPYRIGHT NAME!]
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

#pragma once

#include "Module.h"
#include <interfaces/IPluginAsyncStateControl.h>

namespace Thunder {
namespace Plugin {
    
    class PluginInitializerService : public PluginHost::IPlugin, public Exchange::IPluginAsyncStateControl {
    public:
        PluginInitializerService(const PluginInitializerService&) = delete;
        PluginInitializerService& operator=(const PluginInitializerService&) = delete;
        PluginInitializerService(PluginInitializerService&&) = delete;
        PluginInitializerService& operator=(PluginInitializerService&&) = delete;

PUSH_WARNING(DISABLE_WARNING_THIS_IN_MEMBER_INITIALIZER_LIST)
        PluginInitializerService()
            : PluginHost::IPlugin()
            , Exchange::IPluginAsyncStateControl()
            , _maxparallel(2)
            , _maxretries(5)
            , _delay(500)
            , _pluginInitList()
            , _sink(*this)
            , _service(nullptr)
            , _adminLock()
        {
        }
POP_WARNING()

        ~PluginInitializerService() override = default;
    private:
        class Config : public Core::JSON::Container {
        public:
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;
            Config(Config&&) = delete;
            Config& operator=(Config&&) = delete;
            
            Config()
                : Core::JSON::Container()
                , MaxParallel()
                , MaxRetries(5)
                , Delay(500)
            {
                Add(_T("maxparallel"), &MaxParallel);
                Add(_T("maxretries"), &MaxRetries);
                Add(_T("delay"), &Delay);
            }
            ~Config() override = default;
        public:
            Core::JSON::DecUInt8 MaxParallel;
            Core::JSON::DecUInt8 MaxRetries;
            Core::JSON::DecUInt16 Delay;
        };

    private:

        class PluginStarter {
        public:
            PluginStarter(PluginHost::IShell* requestedPluginShell
                         , uint8_t maxnumberretries
                         , uint16_t delay
                         , IPluginAsyncStateControl::IActivationCallback* callback
                         , PluginInitializerService& initservice)
                : _callsign()
                , _requestedPluginShell(requestedPluginShell)
                , _retries(0)
                , _maxnumberretries(maxnumberretries)
                , _delay(delay)
                , _callback(callback)
                , _initializerservice(initservice)
            {
                ASSERT(_requestedPluginShell != nullptr);
                _callsign = _requestedPluginShell->Callsign();
                _requestedPluginShell->AddRef();
                if (_callback != nullptr) {
                    _callback->AddRef();
                }
            }
            ~PluginStarter()
            {
                if (_requestedPluginShell != nullptr) {
                    _requestedPluginShell->Release();
                    _requestedPluginShell = nullptr;
                }
                if (_callback != nullptr) {
                    _callback->Release();
                    _callback = nullptr;
                }
            }

            PluginStarter(const PluginStarter&) = delete;
            PluginStarter& operator=(const PluginStarter&) = delete;
            PluginStarter(PluginStarter&& other)
                : _callsign(std::move(other._callsign))
                , _requestedPluginShell(other._requestedPluginShell)
                , _retries(other._retries)
                , _maxnumberretries(other._maxnumberretries)
                , _delay(other._delay)
                , _callback(other._callback)
                , _initializerservice(other._initializerservice)
            {
                other._requestedPluginShell = nullptr;
                other._callback = nullptr;
            }
            PluginStarter& operator=(PluginStarter&& other)
            {
                if (this != &other) {
                    _requestedPluginShell = other._requestedPluginShell;
                    other._requestedPluginShell = nullptr;
                    _callback = other._callback;
                    other._callback = nullptr;
                    ASSERT(&_initializerservice == &other._initializerservice); // there should only be one...
                }
                return *this;
            }

            friend bool operator==(const PluginStarter& lhs, const PluginStarter& rhs)
            {
                return lhs._callsign == rhs._callsign;
            }
            friend bool operator!=(const PluginStarter& lhs, const PluginStarter& rhs)
            {
                return !(lhs == rhs);
            }
            friend bool operator==(const PluginStarter& lhs, const string& callsign)
            {
                return lhs._callsign == callsign;
            }
            friend bool operator!=(const PluginStarter& lhs, const string& callsign)
            {
                return !(lhs == callsign);
            }
            friend bool operator==(const string& callsign, const PluginStarter& lhs)
            {
                return lhs._callsign == callsign;
            }
            friend bool operator!=(const string& callsign, const PluginStarter& lhs)
            {
                return !(lhs == callsign);
            }

            void Activate()
            {
                TRACE(Trace::Information, (_T("Start activating plugin [%s]"), Callsign().c_str()));
                ++_retries;
                // we'll not keep the job, when activation is actualy started, aborting after that might not always abort the plugin activation (does not make sense, could always cross eachother anyway, and otherwisse we need to keep the job)
                Core::IWorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(_initializerservice._service, PluginHost::IShell::ACTIVATED, PluginHost::IShell::REQUESTED));
            }
            void Abort()
            {
                if (_callback != nullptr) {
                    _callback->Finished(Callsign(), Exchange::IPluginAsyncStateControl::IActivationCallback::state::ABORTED, _retries);
                }
            }
            const string& Callsign() const
            {
                return _callsign;
            }
            bool Active() const
            {
                ASSERT(_requestedPluginShell != nullptr);
                // if the plugin is waiting for preconditions it is not eating bread here, so we can add another in parallel
                return( ( _retries > 0 ) && ( _requestedPluginShell->State() != PluginHost::IShell::PRECONDITION ) );
            }
            void Activated()
            {
                TRACE(Trace::Information, (_T("Plugin [%s] was acivated"), Callsign().c_str()));
                if (_callback != nullptr) {
                    // huppel to do: do this from a new job to decouple from the notifications job?
                    TRACE(Trace::Information, (_T("Result callback success called for plugin [%s]"), Callsign().c_str()));
                    _callback->Finished(Callsign(), Exchange::IPluginAsyncStateControl::IActivationCallback::state::SUCCESS, _retries);
                }
            }
            void Failed()
            {
            }
        private:
            string _callsign; //as _requestedPluginShell->Callsign(); returns a temp and we need this lots of times let's keep a copy
            PluginHost::IShell* _requestedPluginShell;
            uint8_t _retries;
            uint8_t _maxnumberretries;
            uint16_t _delay;
            IPluginAsyncStateControl::IActivationCallback* _callback;
            PluginInitializerService& _initializerservice;
        };


        class Notifications : public PluginHost::IPlugin::INotification, public PluginHost::IPlugin::ILifeTime {
        public:
            explicit Notifications(PluginInitializerService& initservice)
                : PluginHost::IPlugin::INotification()
                , PluginHost::IPlugin::ILifeTime()
                , _initservice(initservice)
            {
            }
            ~Notifications() override = default;

            Notifications(const Notifications&) = delete;
            Notifications& operator=(const Notifications&) = delete;
            Notifications(Notifications&&) = delete;
            Notifications& operator=(Notifications&&) = delete;

            // IPlugin::INotification overrides
            void Activated(const string& callsign, PluginHost::IShell* plugin VARIABLE_IS_NOT_USED) override
            {
                _initservice.ActivatedNotification(callsign);
            }
            void Deactivated(const string& callsign, PluginHost::IShell* plugin) override
            {

            }
            void Unavailable(const string& callsign, PluginHost::IShell* plugin) override
            {

            }

            // IPlugin::ILifeTime overrides
            void Initialize(const string& callsign, PluginHost::IShell* plugin) override
            {

            }
            void Deinitialized(const string& callsign, PluginHost::IShell* plugin) override
            {

            }

            BEGIN_INTERFACE_MAP(Notifications)
            INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
            INTERFACE_ENTRY(PluginHost::IPlugin::ILifeTime)
            END_INTERFACE_MAP

        private:
            PluginInitializerService& _initservice;
        };
        
    public:
        // IPlugin Methods
        const string Initialize(PluginHost::IShell* service) override;
        void Deinitialize(PluginHost::IShell* service) override;
        string Information() const override;
        
        // IPluginAsyncStateControl methods
        Core::hresult Activate(const string& callsign, const Core::OptionalType<uint8_t>& maxnumberretries, const Core::OptionalType<uint16_t>& delay, IPluginAsyncStateControl::IActivationCallback* const cb) override;
        Core::hresult AbortActivate(const string& callsign) override;
       
        
        BEGIN_INTERFACE_MAP(PluginInitializerService)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(Exchange::IPluginAsyncStateControl)
        END_INTERFACE_MAP

    private:
        void ActivateNotifications()
        {
            ASSERT(_service != nullptr);
            _service->Register(&_sink);
            TRACE(Trace::Information, (_T("Started listening for plugin state notifications")));
        }
        void DeactivateNotifications()
        {
            ASSERT(_service != nullptr);
            _service->Unregister(&_sink);
            TRACE(Trace::Information, (_T("Stopped listening for plugin state notifications")));
        }

        bool NewPluginStarter(PluginHost::IShell* requestedPluginShell, uint8_t maxnumberretries, uint16_t delay, IPluginAsyncStateControl::IActivationCallback* callback)
        {
            bool result = true;
            PluginStarter starter(requestedPluginShell, maxnumberretries, delay, callback, *this);

            _adminLock.Lock();

            //see if this callsign is not yet in the list
            if (std::find(_pluginInitList.cbegin(), _pluginInitList.cend(), starter) == _pluginInitList.cend()) {
                _pluginInitList.emplace_back(std::move(starter));
                if (_pluginInitList.size() == 1) {
                    ActivateNotifications();
                }
            }
            else {
                //oops this callsign was already requested...
                result = false;
            }

            _adminLock.Unlock();
            return result;
        }

        void ActivateAnotherPlugin() 
        {
            _adminLock.Lock();

            PluginStarterContainer::iterator it = _pluginInitList.begin();
            uint16_t currentlyActive = 0;
            while (it != _pluginInitList.end() && currentlyActive < _maxparallel) {
                if (it->Active() == false) {
                    it->Activate();
                    break; // we activated another we can stop looking...
                }
                ++currentlyActive;
                ASSERT(currentlyActive < std::numeric_limits<uint16_t>::max()); // I'll bet this will fire at some point :)
            };

            _adminLock.Unlock();
        }

        bool CancelPluginStarter(const string& callsign)
        {
            bool result = false;

            _adminLock.Lock();

            PluginStarterContainer::iterator it = std::find(_pluginInitList.begin(), _pluginInitList.end(), callsign);
            if (it != _pluginInitList.end()) {
                PluginStarter toAbort(std::move(*it));
                _pluginInitList.erase(it);
                if (_pluginInitList.size() == 0) {
                    DeactivateNotifications();
                }
                _adminLock.Unlock();
                result = true;
                toAbort.Abort();
            } else {
                _adminLock.Unlock();
            }

            return result;
        }

        void ActivatedNotification(const string& callsign)
        {
            _adminLock.Lock();

            PluginStarterContainer::iterator it = std::find(_pluginInitList.begin(), _pluginInitList.end(), callsign);
            if (it != _pluginInitList.end()) {
                // okay this plugin is activated were done for this one!
                PluginStarter activted(std::move(*it));
                _pluginInitList.erase(it);
                _adminLock.Unlock();
                activted.Activated();
            } else {
                _adminLock.Unlock();
            }
        }

    private:
        using PluginStarterContainer = std::list<PluginStarter>; // for now we keep them in a list as we want them to activate them in order receieved (if needed we can add a shadow unordered map for quick lookup but we do not expect that many parallel activation requests, at least I hope...)

        uint8_t                         _maxparallel;
        uint8_t                         _maxretries;
        uint16_t                        _delay;
        PluginStarterContainer          _pluginInitList;
        Core::SinkType<Notifications>   _sink;
        PluginHost::IShell*             _service;
        Core::CriticalSection           _adminLock;
    };
} // Plugin
} // Thunder