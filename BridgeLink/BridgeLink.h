#pragma once

#include "Module.h"

namespace Thunder {
namespace Plugin {

    class EXTERNAL BridgeLink : public PluginHost::IPlugin, public PluginHost::ICompositPlugin {
	private:
		class Link {
   		private:
			class Connector 
                : public PluginHost::IPlugin::INotification
                , public Exchange::Controller::IShells::INotification {
			private:
				class Channel : public RPC::CommunicatorClient {
				public:
					Channel() = delete;
					Channel(Channel&&) = delete;
					Channel(const Channel&) = delete;
					Channel& operator=(Channel&&) = delete;
                    Channel& operator=(const Channel&) = delete;

					Channel(Connector& parent, const Core::NodeId& remoteNode)
						: RPC::CommunicatorClient(remoteNode, Core::ProxyType<Core::IIPCServer>(RPC::WorkerPoolInvokeServer()))
						, _parent(parent)
						,_controller(nullptr)
					{
					}
					~Channel() override {
						ASSERT(_controller == nullptr);
					}

				public:
					bool IsOperational() const {
						return ((_controller != nullptr) && (RPC::CommunicatorClient::IsOpen() == true));
					}
					uint32_t Register(const uint32_t waitTime) {
						uint32_t result = Core::ERROR_INCORRECT_URL;

                        ASSERT(_controller == nullptr);
						ASSERT(CommunicatorClient::Source().IsOpen() == false);

                        PluginHost::IShell* controller = CommunicatorClient::Open< PluginHost::IShell>(_T(""), ~0, waitTime);
                        if (controller == nullptr) {
                            CommunicatorClient::Close(waitTime);
                        }
                        else {
                            Exchange::Controller::IShells* shells = controller->QueryInterface<Exchange::Controller::IShells>();

                            if (shells == nullptr) {
                                controller->Release();
                                controller = nullptr;
                                CommunicatorClient::Close(waitTime);
                                result = Core::ERROR_UNAVAILABLE;
                            }
                            else {
                                _controller = controller;

                                shells->Register(&_parent);
                                controller->Register(&_parent);

                                shells->Release();
                                result = Core::ERROR_NONE;
                            }
						}

						return (result);
					}
					void Unregister() {
						if (_controller != nullptr) {
                            if (CommunicatorClient::Source().IsOpen() == true) {
                                // For the Plugin::INotification
                                _parent.Release();
                                // For the IShells::INotification
                                _parent.Release();
                            }
                            else {
                                Exchange::Controller::IShells* shells = _controller->QueryInterface<Exchange::Controller::IShells>();

                                if (shells == nullptr) {
                                    _parent.Release();
                                }
                                else {
                                    shells->Unregister(&_parent);
                                    shells->Release();
                                }
                                _controller->Unregister(&_parent);
							}
                            _controller->Release();
                            _controller = nullptr;
						}
					}
					void StateChange() override {
						CommunicatorClient::StateChange();
						if ((_controller != nullptr) && (CommunicatorClient::Source().IsOpen() == false)) {
							_parent.Closed();
                            Unregister();
						}
					}

				private:
					Connector& _parent;
					PluginHost::IShell* _controller;
				};

			public:
				Connector() = delete;
				Connector(Connector&&) = delete;
				Connector(const Connector&) = delete;
                Connector& operator=(Connector&&) = delete;
                Connector& operator=(const Connector&) = delete;

				Connector(Link& parent)
					: _parent(parent)
					, _retries(0)
					, _maxRetries(0)
					, _interval(0)
					, _destination()
					, _channel()
					, _job(*this) {
				}
				~Connector() {
					ASSERT(_channel.IsValid() == false);
				}

			public:
				bool IsOperational() const {
					return ((_channel.IsValid() == true) && (_channel->IsOperational()));
				}
				uint32_t Initialize(const string& source, const uint8_t retries, const uint16_t interval) {
                    uint32_t result = Core::ERROR_INCOMPLETE_CONFIG;

					_destination = Core::NodeId(source.c_str());

                    if (_destination.IsValid() == true) {
                        _retries = retries;
                        _maxRetries = retries;
                        _interval = interval;
                        _job.Submit();

                        result = Core::ERROR_NONE;
                    }

                    return (result);
				}
				void Deinitialize() {
					_job.Revoke();

					if (_channel.IsValid() == true) {
						_channel->Unregister();
						_channel->Close(Core::infinite);
						_channel.Release();
					}
				}
				void Closed() {
					_parent.Disconnected();

					// Dive into the retry loop again.. 
					_job.Reschedule(Core::Time::Now().Add(_interval * 1000));
				}
                void Created(const string& callsign, PluginHost::IShell* plugin) override {
                    _parent.Created(callsign, plugin);
                }
                void Destroy(const string& callsign, PluginHost::IShell* plugin) override {
                    _parent.Destroy(callsign, plugin);
                }
                void Activated(const string& callsign, PluginHost::IShell* plugin) override {
                    _parent.Activated(callsign, plugin);
                }
                void Deactivated(const string& callsign, PluginHost::IShell* plugin) override {
                    _parent.Deactivated(callsign, plugin);
                }
                void Unavailable(const string& callsign, PluginHost::IShell* plugin) override {
                    _parent.Unavailable(callsign, plugin);
                }

                BEGIN_INTERFACE_MAP(Connector)
                    INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
                    INTERFACE_ENTRY(Exchange::Controller::IShells::INotification)
                END_INTERFACE_MAP

            private:
                friend class Core::ThreadPool::JobType<Connector&>;

                void Dispatch() {
                    bool opened = false;

                    if (_channel.IsValid() == false) {
                        _channel = Core::ProxyType<Channel>::Create(*this, _destination);
                    }
                    if (_channel.IsValid() == true) {
                        if (_channel->Register(1000) == Core::ERROR_NONE) {
                            TRACE(Trace::Information, (_T("Connect the link to: [%s], received: [%d]"), _destination.HostName().c_str(), _parent.Plugins()));
                            _retries = _maxRetries;
                            opened = true;
                            _parent.Connected();
                        }
                        else {
                            TRACE(Trace::Information, (_T("Failed to connect the link to: [%s]"), _destination.HostName().c_str()));
                        }
                    }

                    if ((opened == false) && (_retries > 0)) {
                        if (_retries != 0xFF) {
                            _retries--;
                        }
                        _job.Reschedule(Core::Time::Now().Add(_interval * 1000));
                    }
                }

            private:
                Link& _parent;
                uint8_t _retries;
                uint8_t _maxRetries;
                uint16_t _interval;
                Core::NodeId _destination;
                Core::ProxyType<Channel> _channel;
                Core::WorkerPool::JobType<Connector&> _job;
            };
            class ShellProxy :
                public PluginHost::IShell,
                public PluginHost::IDispatcher {
            public:
                enum state : uint8_t {
                    UNAVAILABLE = PluginHost::IShell::state::UNAVAILABLE,
                    DEACTIVATED = PluginHost::IShell::state::DEACTIVATED,
                    ACTIVATED = PluginHost::IShell::state::ACTIVATED
                };

                ShellProxy() = delete;
                ShellProxy(ShellProxy&&) = delete;
                ShellProxy(const ShellProxy&) = delete;
                ShellProxy& operator= (ShellProxy&&) = delete;
                ShellProxy& operator= (const ShellProxy&) = delete;

                ShellProxy(PluginHost::IShell* real, const string& callsign)
                    : _adminLock()
                    , _shell(real)
                    , _dispatcher(nullptr)
                    , _state(static_cast<state>(~0))
                    , _report(static_cast<state>(~0))
                    , _callsign(callsign) {
                    _shell->AddRef();
                }
                ~ShellProxy() override {
                    if (_dispatcher != nullptr) {
                        _dispatcher->Release();
                        _dispatcher = nullptr;
                    }
                    if (_shell != nullptr) {
                        _shell->Release();
                        _shell = nullptr;
                    }
                }

            public:
                void Load() {
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        PluginHost::IShell::state result = source->State();
                        if ((result != PluginHost::IShell::state::ACTIVATED) && (result != PluginHost::IShell::state::UNAVAILABLE)) {
                            _report = state::DEACTIVATED;
                        }
                        else {
                            _report = static_cast<state>(result);
                        }
                        source->Release();
                    }
                }
                bool IsSource(const PluginHost::IShell* shell) const {
                    ASSERT(shell != nullptr);
                    return(shell == _shell);
                }
                void Unlink() {
                    ASSERT(_shell != nullptr);
                    _adminLock.Lock();
                    if (_shell != nullptr) {
                        _shell->Release();
                        _shell = nullptr;
                    }
                    _adminLock.Unlock();
                }
                bool Activate() {
                    bool changed = false;
                    _adminLock.Lock();
                    if (_state != state::ACTIVATED) {
                        _report = state::ACTIVATED;
                        changed = true;
                    }
                    _adminLock.Unlock();
                    return (changed);
                }
                bool Deactivate() {
                    bool changed = false;
                    _adminLock.Lock();
                    if (_state != state::DEACTIVATED) {
                        _report = state::DEACTIVATED;
                        changed = true;
                    }
                    _adminLock.Unlock();
                    return (changed);
                }
                bool Unavailable() {
                    bool changed = false;
                    _adminLock.Lock();
                    if (_state != state::UNAVAILABLE) {
                        _report = state::UNAVAILABLE;
                        changed = true;
                    }
                    _adminLock.Unlock();
                    return (changed);
                }
                Core::OptionalType<state> Report() {

                    Core::OptionalType<state> result;

                    _adminLock.Lock();

                    if (_report != _state) {
                        _state = _report;
                        result = _report;

                        if (_state == state::ACTIVATED) {

                            ASSERT(_dispatcher == nullptr);

                            if (_dispatcher == nullptr) {
                                _dispatcher = _shell->QueryInterface<PluginHost::IDispatcher>();
                            }
                        }
                        else if (_dispatcher != nullptr) {
                            _dispatcher->Release();
                            _dispatcher = nullptr;
                        }
                    }

                    _adminLock.Unlock();

                    return(result);
                }
                void EnableWebServer(const string& URLPath, const string& fileSystemPath) override {
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        source->EnableWebServer(URLPath, fileSystemPath);
                        source->Release();
                    }
                }
                void DisableWebServer() override {
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        source->DisableWebServer();
                        source->Release();
                    }
                }
                //! Model: Returns a Human Readable name for the platform it is running on.
                string Model() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Model();
                        source->Release();
                    }
                    return (result);
                }
                //! Background: If enabled, the PluginHost is running in daemon mode
                bool Background() const override {
                    bool result = true;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Background();
                        source->Release();
                    }
                    return (result);
                }
                //! Accessor: Identifier that can be used for Core:NodeId to connect to the webbridge.
                string Accessor() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Accessor();
                        source->Release();
                    }
                    return (result);
                }
                //! WebPrefix: First part of the pathname in the HTTP request to select the webbridge components.
                string WebPrefix() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->WebPrefix();
                        source->Release();
                    }
                    return (result);
                }
                //! Locator: The name of the binary (so) that holds the given ClassName code.
                string Locator() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Locator();
                        source->Release();
                    }
                    return (result);
                }
                //! ClassName: Name of the class to be instantiated for this IShell
                string ClassName() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->ClassName();
                        source->Release();
                    }
                    return (result);
                }
                //! Versions: Returns a JSON Array of versions (JSONRPC interfaces) supported by this plugin.
                string Versions() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Versions();
                        source->Release();
                    }
                    return (result);
                }
                //! Callsign: Instantiation name of this specific plugin. It is the name given in the config for the classname.
                string Callsign() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Callsign();
                        source->Release();
                    }
                    return (result);
                }
                //! PersistentPath: <config:persistentpath>/<plugin:callsign>/
                string PersistentPath() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->PersistentPath();
                        source->Release();
                    }
                    return (result);
                }
                //! VolatilePath: <config:volatilepath>/<plugin:callsign>/
                string VolatilePath() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->VolatilePath();
                        source->Release();
                    }
                    return (result);
                }
                //! DataPath: <config:datapath>/<plugin:classname>/
                string DataPath() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->DataPath();
                        source->Release();
                    }
                    return (result);
                }
                //! ProxyStubPath: <config:proxystubpath>/
                string ProxyStubPath() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->ProxyStubPath();
                        source->Release();
                    }
                    return (result);
                }
                //! SystemPath: <config:systempath>/
                string SystemPath() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->SystemPath();
                        source->Release();
                    }
                    return (result);
                }
                //! SystemPath: <config:apppath>/Plugins/
                string PluginPath() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->PluginPath();
                        source->Release();
                    }
                    return (result);
                }
                //! SystemPath: <config:systemrootpath>/
                string SystemRootPath() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->SystemRootPath();
                        source->Release();
                    }
                    return (result);
                }
                //! SystemRootPath: Set <config:systemrootpath>/
                uint32_t SystemRootPath(const string& systemRootPath) override {
                    uint32_t result = Core::ERROR_UNAVAILABLE;
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->SystemRootPath(systemRootPath);
                        source->Release();
                    }
                    return (result);
                }
                //! GetLibrarySearchPaths: Get configured pats to search for plugin.
                RPC::IStringIterator* GetLibrarySearchPaths(const string& pathCriteria) const override {
                    RPC::IStringIterator* result = nullptr;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->GetLibrarySearchPaths(pathCriteria);
                        source->Release();
                    }
                    return (result);
                }
                //! Startup: <config:startup>/
                PluginHost::IShell::startmode StartMode() const override {
                    PluginHost::IShell::startmode result = PluginHost::IShell::startmode::UNAVAILABLE;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->StartMode();
                        source->Release();
                    }
                    return (result);
                }
                //! Startup: Set<startup,startmode,resumed states>/
                uint32_t StartMode(const PluginHost::IShell::startmode value) override {
                    uint32_t result = Core::ERROR_UNAVAILABLE;
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->StartMode(value);
                        source->Release();
                    }
                    return (result);
                }
                //! Substituted Config value
                string Substitute(const string& input) const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Substitute(input);
                        source->Release();
                    }
                    return (result);
                }
                bool Resumed() const override {
                    bool result = true;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Resumed();
                        source->Release();
                    }
                    return (result);
                }
                uint32_t Resumed(const bool value) override {
                    uint32_t result = Core::ERROR_UNAVAILABLE;
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Resumed(value);
                        source->Release();
                    }
                    return (result);
                }
                string HashKey() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->HashKey();
                        source->Release();
                    }
                    return (result);
                }
                string ConfigLine() const override {
                    string result;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->SystemRootPath();
                        source->Release();
                    }
                    return (result);
                }
                uint32_t ConfigLine(const string& config) override {
                    uint32_t result = Core::ERROR_UNAVAILABLE;
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->ConfigLine(config);
                        source->Release();
                    }
                    return (result);
                }
                //! Return whether the given version is supported by this IShell instance.
                bool IsSupported(const uint8_t version) const override {
                    bool result = true;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->IsSupported(version);
                        source->Release();
                    }
                    return (result);
                }
                // Get access to the SubSystems and their corrresponding information. Information can be set or get to see what the
                // status of the sub systems is.
                PluginHost::ISubSystem* SubSystems() override {
                    PluginHost::ISubSystem* result = nullptr;
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->SubSystems();
                        source->Release();
                    }
                    return (result);
                }
                // Notify all subscribers of this service with the given string.
                // It is expected to be JSON formatted strings as it is assumed that this is for reaching websockets clients living in
                // the web world that have build in functionality to parse JSON structs.
                void Notify(const string& part1, const string& message) override {
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        source->Notify(part1, message);
                        source->Release();
                    }
                }
                // Allow access to the Shells, configured for the different Plugins found in the configuration.
                // Calling the QueryInterfaceByCallsign with an empty callsign will query for interfaces located
                // on the controller.
                void Register(IPlugin::INotification* sink, const Core::OptionalType<string>& callsign) override {
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        source->Register(sink, callsign);
                        source->Release();
                    }
                }
                void Unregister(IPlugin::INotification* sink, const Core::OptionalType<string>& callsign) override {
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        source->Unregister(sink, callsign);
                        source->Release();
                    }
                }
                IShell::state State() const override {
                    Core::SafeSyncType<Core::CriticalSection> lock(_adminLock);
                    return (_shell == nullptr ? IShell::state::DESTROYED : static_cast< IShell::state>(_state));
                }
                void* /* @interface:id */ QueryInterfaceByCallsign(const uint32_t id, const string& name) override {
                    void* result = nullptr;
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->QueryInterfaceByCallsign(id, name);
                        source->Release();
                    }
                    return (result);
                }
                // Methods to Activate/Deactivate and Unavailable the aggregated Plugin to this shell.
                // NOTE: These are Blocking calls!!!!!
                uint32_t Activate(const reason why) override {
                    uint32_t result = Core::ERROR_UNAVAILABLE;
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Activate(why);
                        source->Release();
                    }
                    return (result);
                }
                uint32_t Deactivate(const reason why) override {
                    uint32_t result = Core::ERROR_UNAVAILABLE;
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Deactivate(why);
                        source->Release();
                    }
                    return (result);
                }
                uint32_t Unavailable(const reason why) override {
                    uint32_t result = Core::ERROR_UNAVAILABLE;
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Unavailable(why);
                        source->Release();
                    }
                    return (result);
                }
                uint32_t Hibernate(const uint32_t timeout) override {
                    uint32_t result = Core::ERROR_UNAVAILABLE;
                    PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Hibernate(timeout);
                        source->Release();
                    }
                    return (result);
                }
                reason Reason() const override {
                    reason result = reason::FAILURE;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Reason();
                        source->Release();
                    }
                    return (result);
                }
                Core::hresult Metadata(string& info /* @out */) const {
                    Core::hresult result = Core::ERROR_UNAVAILABLE;
                    const PluginHost::IShell* source = Source();
                    if (source != nullptr) {
                        result = source->Metadata(info);
                        source->Release();
                    }
                    return (result);
                }

                //
                // Implementation IDispatcher
                // ------------------------------------------------------------------------------------------
                uint32_t Invoke(const uint32_t channelid, const uint32_t id, const string& token, const string& method, const string& parameters, string& response /* @out */) override {
                    uint32_t result = Core::ERROR_UNAVAILABLE;

                    _adminLock.Lock();
                    PluginHost::IDispatcher* link = _dispatcher;
                    if (link != nullptr) {
                        link->AddRef();
                    }
                    _adminLock.Unlock();

                    if ((link != nullptr) && (method.length() > _callsign.length())) {
                        ASSERT(method.at(_callsign.length()) == PluginHost::ICompositPlugin::Delimiter);
                        // Do not forget to takeof "our" callsign
                        string strippedMethod = method.substr(_callsign.length() + 1);
                        result = link->Invoke(channelid, id, token, strippedMethod, parameters, response);
                        link->Release();
                    }
                    return (result);
                }
                Core::hresult Subscribe(ICallback* callback, const string& event, const string& designator) override {
                    uint32_t result = Core::ERROR_UNAVAILABLE;

                    _adminLock.Lock();
                    PluginHost::IDispatcher* link = _dispatcher;
                    if (link != nullptr) {
                        link->AddRef();
                    }
                    _adminLock.Unlock();

                    if (link != nullptr) {
                        result = link->Subscribe(callback, event, designator);
                        link->Release();
                    }
                    return (result);
                }
                Core::hresult Unsubscribe(ICallback* callback, const string& event, const string& designator) override {
                    uint32_t result = Core::ERROR_UNAVAILABLE;

                    _adminLock.Lock();
                    PluginHost::IDispatcher* link = _dispatcher;
                    if (link != nullptr) {
                        link->AddRef();
                    }
                    _adminLock.Unlock();

                    if (link != nullptr) {
                        result = link->Unsubscribe(callback, event, designator);
                        link->Release();
                    }
                    return (result);
                }
                Core::hresult Attach(IShell::IConnectionServer::INotification*& sink /* @out */, IShell* service) override {
                    return (Core::ERROR_UNAVAILABLE);
                }
                Core::hresult Detach(IShell::IConnectionServer::INotification*& sink /* @out */) override {
                    return (Core::ERROR_UNAVAILABLE);
                }
                void Dropped(const ICallback* callback) override {
                }
                void* QueryInterface(const uint32_t interfaceNumber) override {
                    void* result = nullptr;
                    if (interfaceNumber == Core::IUnknown::ID) {
                        AddRef();
                        result = static_cast<void*>(static_cast<Core::IUnknown*>(this));
                    }
                    else if (interfaceNumber == PluginHost::IShell::ID) {
                        AddRef();
                        result = static_cast<void*>(static_cast<PluginHost::IShell*>(this));
                    }
                    else if (interfaceNumber == PluginHost::IDispatcher::ID) {
                        _adminLock.Lock();
                        if (_dispatcher != nullptr) {
                            AddRef();
                            result = static_cast<void*>(static_cast<PluginHost::IDispatcher*>(this));
                        }
                        _adminLock.Unlock();
                    }
                    else {
                        PluginHost::IShell* source = Source();
                        if (source != nullptr) {
                            result = source->QueryInterface(interfaceNumber);
                            source->Release();
                        }
                    }
                    return (result);
                }
                PluginHost::IShell* Source() {
                    _adminLock.Lock();
                    PluginHost::IShell* result = _shell;
                    if (result != nullptr) {
                        result->AddRef();
                    }
                    _adminLock.Unlock();
                    return (result);
                }
                const PluginHost::IShell* Source() const {
                    _adminLock.Lock();
                    const PluginHost::IShell* result = _shell;
                    if (result != nullptr) {
                        result->AddRef();
                    }
                    _adminLock.Unlock();
                    return (result);
                }

            private:
                mutable Core::CriticalSection _adminLock;
                PluginHost::IShell* _shell;
                PluginHost::IDispatcher* _dispatcher;
                state _state;
                state _report;
                const string _callsign;
            };

            using Pendings = std::unordered_map<string, PluginHost::IShell*>;
            using Shells = std::unordered_map<string, ShellProxy*>;

		public:
			Link() = delete;
			Link(Link&&) = delete;
			Link(const Link&) = delete;
            Link& operator=(Link&&) = delete;
            Link& operator=(const Link&) = delete;

			PUSH_WARNING(DISABLE_WARNING_THIS_IN_MEMBER_INITIALIZER_LIST);
			Link(BridgeLink& parent)
				: _parent(parent) 
                , _adminLock()
                , _isEmpty(false, true)
                , _callback(nullptr)
                , _created()
                , _destructed()
                , _externalShells() 
				, _connection(*this)
                , _job(*this) {
			}
			POP_WARNING();
			~Link() = default;

		public:
            uint32_t Plugins() const {
                return (static_cast<uint32_t>(_externalShells.size()));
            }
            void Callback(PluginHost::ICompositPlugin::ICallback* sink) {

                ASSERT((sink != nullptr) ^ (_callback != nullptr));

                _adminLock.Lock();
                if (_callback != nullptr) {
                    _callback->Release();
                }
                _callback = sink;
                if (_callback != nullptr) {
                    _callback->AddRef();
                    for (const std::pair<string, ShellProxy*>& entry : _externalShells) {
                        _callback->Created(entry.first, entry.second);
                        if (entry.second->State() == PluginHost::IShell::ACTIVATED) {
                            _callback->Activated(entry.first, entry.second);
                        }
                        else if (entry.second->State() == PluginHost::IShell::UNAVAILABLE) {
                            _callback->Unavailable(entry.first, entry.second);
                        }
                        else {
                            _callback->Deactivated(entry.first, entry.second);
                        }
                    }
                }
                _adminLock.Unlock();
            }
			uint32_t Initialize(const string& source, const uint8_t retries, const uint16_t interval) {
				_connection.Initialize(source, retries, interval);
                return (Core::ERROR_NONE);
			}
			uint32_t Deinitialize(const uint32_t waitTime) {
				_connection.Deinitialize();
				Flush();
                return (_isEmpty.Lock(waitTime));
			}

        private:
            friend Core::ThreadPool::JobType<Link&>;

            void Connected() {
            }
            void Disconnected() {
                Flush();
            }
            void Created(const string& callsign, PluginHost::IShell* plugin) {

                _adminLock.Lock();

				Pendings::iterator entry = _created.find(callsign);

                ASSERT(_externalShells.find(callsign) == _externalShells.end());
                ASSERT(entry == _created.end());

				if (entry == _created.end()) {
                    _created.emplace(std::piecewise_construct,
                        std::forward_as_tuple(callsign),
                        std::forward_as_tuple(plugin));
                    plugin->AddRef();
                    _isEmpty.ResetEvent();
				}

                _adminLock.Unlock();

                _job.Submit();
            }
			void Destroy(const string& callsign, PluginHost::IShell* plugin) {

                _adminLock.Lock();

                Pendings::iterator entry = _created.find(callsign);

                if (entry != _created.end()) {
                    // It is Detroyed before we reported is :-)
                    ASSERT(entry->second == plugin);
                    _created.erase(entry);

                    if (_externalShells.empty() && _created.empty() && _destructed.empty()) {
                        _isEmpty.SetEvent();
                    }
                }
                else {
                    Shells::iterator entry = _externalShells.find(callsign);

                    ASSERT(entry != _externalShells.end());

                    if (entry != _externalShells.end()) {
                        // It is detroyed, move the reported element to the destruction queue
                        ASSERT(entry->second->IsSource(plugin));
                        entry->second->Unlink();
                        PluginHost::IShell* proxy = entry->second;
                        proxy->AddRef();
                        _externalShells.erase(entry);
                        _destructed.emplace(std::piecewise_construct,
                            std::forward_as_tuple(callsign),
                            std::forward_as_tuple(proxy));
                    }
                }

                _adminLock.Unlock();

                _job.Submit();
            }
            void Activated(const string& callsign, PluginHost::IShell* plugin) {
                bool change = false;

                _adminLock.Lock();

                Shells::iterator entry = _externalShells.find(callsign);

                if (entry != _externalShells.end()) {
                    ASSERT(entry->second->IsSource(plugin));

                    change = entry->second->Activate();
                }

                _adminLock.Unlock();

                if (change == true) {
                    _job.Submit();
                }
            }
            void Deactivated(const string& callsign, PluginHost::IShell* plugin) {
                bool change = false;

                _adminLock.Lock();

                Shells::iterator entry = _externalShells.find(callsign);

                if (entry != _externalShells.end()) {
                    ASSERT(entry->second->IsSource(plugin));

                    change = entry->second->Deactivate();
                }

                _adminLock.Unlock();

                if (change == true) {
                    _job.Submit();
                }
            }
            void Unavailable(const string& callsign, PluginHost::IShell* plugin) {
                bool change = false;

                _adminLock.Lock();

                Shells::iterator entry = _externalShells.find(callsign);

                if (entry != _externalShells.end()) {
                    ASSERT(entry->second->IsSource(plugin));

                    change = entry->second->Unavailable();
                }

                _adminLock.Unlock();

                if (change == true) {
                    _job.Submit();
                }
            }
            void Dispatch() {
 
                // First report all Destroyed elements
                _adminLock.Lock();

                while (_destructed.empty() == false) {
                    string callsign(_destructed.begin()->first);
                    PluginHost::IShell* shell(_destructed.begin()->second);
                    _destructed.erase(_destructed.begin());

                    // Unsubscribing (resetting the _callback to a nullptr) requires the Job to be revoked.
                    // If the Job is running the revoke will wait till it is completed before restting it 
                    // to a nullptr!
                    if (_callback != nullptr) {
                        _adminLock.Unlock();

                        _callback->Destroy(callsign, shell);

                        _adminLock.Lock();
                    }
                    shell->Release();
                }

                // Than report all Created elements
                while (_created.empty() == false) {
                    string callsign(_created.begin()->first);
                    PluginHost::IShell* shell(_created.begin()->second);
                    ShellProxy* newEntry = Core::ServiceType<ShellProxy>::Create<ShellProxy>(shell, _parent.Callsign());
                    _created.erase(_created.begin());
                    shell->Release();

                    _externalShells.emplace(std::piecewise_construct,
                        std::forward_as_tuple(callsign),
                        std::forward_as_tuple(newEntry));

                    // Unsubscribing (resetting the _callback to a nullptr) requires the Job to be revoked.
                    // If the Job is running the revoke will wait till it is completed before restting it 
                    // to a nullptr!
                    if (_callback != nullptr) {
                        _adminLock.Unlock();
                        newEntry->Load();
                        _callback->Created(callsign, newEntry);
                        _adminLock.Lock();
                    }
                }

                // Finally report all state changes
                Shells::iterator entry(_externalShells.begin());

                while (entry != _externalShells.end()) {
                    Core::OptionalType<ShellProxy::state> action = entry->second->Report();

                    // Seems like this entry needs some attention :-)
                    if (action.IsSet() == false) {
                        entry++;
                    }
                    else {
                        // Unsubscribing (resetting the _callback to a nullptr) requires the Job to be revoked.
                        // If the Job is running the revoke will wait till it is completed before restting it 
                        // to a nullptr!
                        if (_callback != nullptr) {
                            string callsign(entry->first);
                            PluginHost::IShell* shell(entry->second);
                            shell->AddRef();

                            _adminLock.Unlock();

                            if (action == ShellProxy::state::ACTIVATED) {
                                _callback->Activated(callsign, shell);
                            }
                            else if (action == ShellProxy::state::DEACTIVATED) {
                                _callback->Deactivated(callsign, shell);
                            }
                            else if (action == ShellProxy::state::UNAVAILABLE) {
                                _callback->Unavailable(callsign, shell);
                            }

                            _adminLock.Lock();

                            shell->Release();
                            entry = _externalShells.begin();
                        }
                    }
                }

                if (_externalShells.empty() && _created.empty() && _destructed.empty()) {
                    _isEmpty.SetEvent();
                }

                _adminLock.Unlock();
            }
            void Flush() {
                _adminLock.Lock();

                for (auto entry : _created) {
                    entry.second->Release();
                }
                _created.clear();

                // Move all active elemnets to destructed so they are reported "Destroyed"

                for (const std::pair<string, ShellProxy*>& entry : _externalShells) {
                    entry.second->Unlink();
                    PluginHost::IShell* proxy = entry.second;
                    // No need to AddRef, in a moment we will kill the full list and those
                    // AddRefs (+1) are taken over by the _detrcuted list..
                    // proxy->AddRef();
                    _destructed.emplace(std::piecewise_construct,
                        std::forward_as_tuple(entry.first),
                        std::forward_as_tuple(proxy));
                }

                // Addrefs from the elements in the map (second element) are now moved to the
                // _detrcuted map from there they get released..
                _externalShells.clear();

                bool dispatchNeeded = (_destructed.empty() == false);

                _adminLock.Unlock();

                if (dispatchNeeded == true) {
                    _job.Submit();
                }
            }

		private:
            BridgeLink& _parent;
            Core::CriticalSection _adminLock;
            Core::Event _isEmpty;
            PluginHost::ICompositPlugin::ICallback* _callback;
            Pendings _created;
            Pendings _destructed;
            Shells _externalShells;
			Core::SinkType<Connector> _connection;
            Core::WorkerPool::JobType<Link&> _job;
		};

    public:
 		class Config : public Core::JSON::Container {
		public:
			Config(Config&&) = delete;
			Config(const Config&) = delete;
			Config& operator=(Config&&) = delete;
            Config& operator=(const Config&) = delete;

			Config()
				: Source()
				, Retries(3)
				, Interval(10)
			{
				Add(_T("source"), &Source);
				Add(_T("retries"), &Retries);
				Add(_T("interval"), &Interval);
			}
			~Config() override = default;

		public:
			Core::JSON::String Source;
			Core::JSON::DecUInt8 Retries;
			Core::JSON::DecUInt8 Interval;
		};

    public:
		BridgeLink(BridgeLink&&) = delete;
		BridgeLink(const BridgeLink&) = delete;
        BridgeLink& operator=(BridgeLink&&) = delete;
        BridgeLink& operator=(const BridgeLink&) = delete;

		PUSH_WARNING(DISABLE_WARNING_THIS_IN_MEMBER_INITIALIZER_LIST);
		BridgeLink()
            : _adminLock()
			, _skipURL(0)
			, _link(*this)
            , _callsign()
        {
        }
		POP_WARNING();
		~BridgeLink() override = default;

    public:
        //   IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        const string Initialize(PluginHost::IShell* service) override;
        void Deinitialize(PluginHost::IShell* service) override;
        string Information() const override;

		//   ICompositPlugin methods
		// -------------------------------------------------------------------------------------------------------
		uint32_t Callback(PluginHost::ICompositPlugin::ICallback* sink) override {
			_link.Callback(sink);
			return (Core::ERROR_NONE);
		}
        string JobIdentifier() const {
            return (_T("Plugin::BridgeLink::Notification"));
        }

		BEGIN_INTERFACE_MAP(BridgeLink)
			INTERFACE_ENTRY(PluginHost::IPlugin)
			INTERFACE_ENTRY(PluginHost::ICompositPlugin)
		END_INTERFACE_MAP

    private:
        const string& Callsign() {
            return (_callsign);
        }

    private:
		Core::CriticalSection _adminLock;
        uint8_t _skipURL;
		Core::SinkType<Link> _link;
        string _callsign;
	};

} // namespace Plugin
} // namespace Thunder

