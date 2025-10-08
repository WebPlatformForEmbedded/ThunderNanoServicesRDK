#include "BridgeLink.h"

namespace Thunder
{
    namespace Plugin {

        namespace {

            static Metadata<BridgeLink> metadata(
                // Version
                1, 0, 0,
                // Preconditions
                {},
                // Terminations
                {},
                // Controls
                {} 
            );
        }

        /* virtual */ const string BridgeLink::Initialize(PluginHost::IShell* service)
        {
            ASSERT (service != nullptr);
            string message;
            Config config;
            config.FromString(service->ConfigLine());
            _skipURL = static_cast<uint8_t>(service->WebPrefix().length());

            if (config.Source.Value().empty() == true) {
                message = _T("There is no address we can link to");
            }
            else {
                _link.Initialize(config.Source.Value(), config.Retries.Value(), config.Interval.Value());
                _callsign = service->Callsign();
            }
			return (EMPTY_STRING);
		}

        /* virtual */ void BridgeLink::Deinitialize(PluginHost::IShell* service)
        {
            _link.Deinitialize(Core::infinite);
            _callsign.clear();
		}

        /* virtual */ string BridgeLink::Information() const
        {
            // No additional info to report.
            return (string());
        }
	} // namespace Plugin
} // namespace Thunder
