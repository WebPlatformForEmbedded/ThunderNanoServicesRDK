#include "DeviceInfoImplementation.h"

namespace Thunder {
namespace Plugin {
    SERVICE_REGISTRATION(DeviceInfoImplementation, 1, 0)

    using AudioCapabilitiesJsonArray = Core::JSON::ArrayType<Core::JSON::EnumType<Exchange::IDeviceAudioCapabilities::AudioCapability>>;
    using MS12CapabilitiesJsonArray = Core::JSON::ArrayType<Core::JSON::EnumType<Exchange::IDeviceAudioCapabilities::MS12Capability>>;
    using MS12ProfilesJsonArray = Core::JSON::ArrayType<Core::JSON::EnumType<Exchange::IDeviceAudioCapabilities::MS12Profile>>;
    using AudioCapabilityIteratorImplementation = RPC::IteratorType<Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator>;
    using MS12CapabilityIteratorImplementation = RPC::IteratorType<Exchange::IDeviceAudioCapabilities::IMS12CapabilityIterator>;
    using MS12ProfileIteratorImplementation = RPC::IteratorType<Exchange::IDeviceAudioCapabilities::IMS12ProfileIterator>;

    using ResolutionJsonArray = Core::JSON::ArrayType<Core::JSON::EnumType<Exchange::IDeviceVideoCapabilities::ScreenResolution>>;
    using ResolutionIteratorImplementation = RPC::IteratorType<Exchange::IDeviceVideoCapabilities::IScreenResolutionIterator>;

    uint32_t DeviceInfoImplementation::Configure(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);

        _config.FromString(service->ConfigLine());

        _supportsHdr = _config.Hdr.Value();
        _supportsAtmos = _config.Atmos.Value();
        _supportsCEC = _config.Cec.Value();
        _hostEdid = _config.Edid.Value();
        _make = _config.Make.Value();
        _deviceType = _config.DeviceType.Value();
        _distributorId = _config.DistributorId.Value();
        _modelName = _config.ModelName.Value();
        _modelYear = _config.ModelYear.Value();
        _friendlyName = _config.FriendlyName.Value();
        _platformName = _config.PlatformName.Value();
        _serialNumber = _config.SerialNumber.Value();
        _sku = _config.Sku.Value();

        Core::JSON::ArrayType<Config::AudioOutput>::Iterator audioOutputIterator(_config.AudioOutputs.Elements());
        while (audioOutputIterator.Next()) {
            AudioOutputCapability audioOutputCapability;

            AudioCapabilitiesJsonArray::Iterator audioCapabilitiesIterator(audioOutputIterator.Current().AudioCapabilities.Elements());
            while (audioCapabilitiesIterator.Next()) {
                audioOutputCapability.AudioCapabilities.push_back(audioCapabilitiesIterator.Current().Value());
            }
            MS12CapabilitiesJsonArray::Iterator ms12CapabilitiesIterator(audioOutputIterator.Current().MS12Capabilities.Elements());
            while (ms12CapabilitiesIterator.Next()) {
                audioOutputCapability.MS12Capabilities.push_back(ms12CapabilitiesIterator.Current().Value());
            }
            MS12ProfilesJsonArray::Iterator ms12ProfilesIterator(audioOutputIterator.Current().MS12Profiles.Elements());
            while (ms12ProfilesIterator.Next()) {
                audioOutputCapability.MS12Profiles.push_back(ms12ProfilesIterator.Current().Value());
            }
            _audioOutputMap.insert(std::pair<Exchange::IDeviceAudioCapabilities::AudioOutput, AudioOutputCapability>
                                   (audioOutputIterator.Current().Name.Value(), audioOutputCapability));
        }

        Core::JSON::ArrayType<Config::VideoOutput>::Iterator videoOutputIterator(_config.VideoOutputs.Elements());
        while (videoOutputIterator.Next()) {
            VideoOutputCapability videoOutputCapability;
            videoOutputCapability.CopyProtection = (videoOutputIterator.Current().CopyProtection.Value());
            videoOutputCapability.DefaultResolution = (videoOutputIterator.Current().DefaultResolution.Value());

            ResolutionJsonArray::Iterator resolutionIterator(videoOutputIterator.Current().Resolutions.Elements());
            while (resolutionIterator.Next()) {
                videoOutputCapability.Resolutions.push_back(resolutionIterator.Current().Value());
            }
            _videoOutputMap.insert(std::pair<Exchange::IDeviceVideoCapabilities::VideoOutput, VideoOutputCapability>
                                   (videoOutputIterator.Current().Name.Value(), videoOutputCapability));
        }

        return (Core::ERROR_NONE);
    }

    Core::hresult DeviceInfoImplementation::AudioOutputs(Exchange::IDeviceAudioCapabilities::IAudioOutputIterator*& audioOutputs) const
    {
        AudioOutputList audioOutputList;

        std::transform(_audioOutputMap.begin(), _audioOutputMap.end(), std::front_inserter(audioOutputList),
        [](decltype(_audioOutputMap)::value_type const &pair) {
            return pair.first;
        });

        audioOutputs = Core::ServiceType<AudioOutputIteratorImplementation>::Create<Exchange::IDeviceAudioCapabilities::IAudioOutputIterator>(audioOutputList);
        return (audioOutputs != nullptr ? Core::ERROR_NONE : Core::ERROR_GENERAL);
    }

    Core::hresult DeviceInfoImplementation::AudioCapabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator*& audioCapabilities) const
    {
        AudioOutputMap::const_iterator index = _audioOutputMap.find(audioOutput);
        if ((index != _audioOutputMap.end()) && (index->second.AudioCapabilities.size() > 0)) {
             audioCapabilities = Core::ServiceType<AudioCapabilityIteratorImplementation>::Create<Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator>(index->second.AudioCapabilities);
        }
        return (audioCapabilities != nullptr ? Core::ERROR_NONE : Core::ERROR_GENERAL);
    }

    Core::hresult DeviceInfoImplementation::MS12Capabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IMS12CapabilityIterator*& ms12Capabilities) const
    {
        AudioOutputMap::const_iterator index = _audioOutputMap.find(audioOutput);
        if ((index != _audioOutputMap.end()) && (index->second.MS12Capabilities.size() > 0)) {
             ms12Capabilities = Core::ServiceType<MS12CapabilityIteratorImplementation>::Create<Exchange::IDeviceAudioCapabilities::IMS12CapabilityIterator>(index->second.MS12Capabilities);
        }
        return (ms12Capabilities != nullptr ? Core::ERROR_NONE : Core::ERROR_GENERAL);
    }

    Core::hresult DeviceInfoImplementation::MS12AudioProfiles(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IMS12ProfileIterator*& ms12AudioProfiles) const
    {
        AudioOutputMap::const_iterator index = _audioOutputMap.find(audioOutput);
        if ((index != _audioOutputMap.end()) && (index->second.MS12Profiles.size() > 0)) {
             ms12AudioProfiles = Core::ServiceType<MS12ProfileIteratorImplementation>::Create<Exchange::IDeviceAudioCapabilities::IMS12ProfileIterator>(index->second.MS12Profiles);
        }
        return (ms12AudioProfiles != nullptr ? Core::ERROR_NONE : Core::ERROR_GENERAL);
    }

    Core::hresult DeviceInfoImplementation::VideoOutputs(Exchange::IDeviceVideoCapabilities::IVideoOutputIterator*& videoOutputs) const
    {
        VideoOutputList videoOutputList;

        std::transform(_videoOutputMap.begin(), _videoOutputMap.end(), std::back_inserter(videoOutputList),
        [](decltype(_videoOutputMap)::value_type const &pair) {
            return pair.first;
        });

        videoOutputs = Core::ServiceType<VideoOutputIteratorImplementation>::Create<Exchange::IDeviceVideoCapabilities::IVideoOutputIterator>(videoOutputList);

        return (videoOutputs != nullptr ? Core::ERROR_NONE : Core::ERROR_GENERAL);
    }

    Core::hresult DeviceInfoImplementation::DefaultResolution(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, Exchange::IDeviceVideoCapabilities::ScreenResolution& defaultResolution) const
    {
        Core::hresult result = Core::ERROR_UNKNOWN_KEY;

        VideoOutputMap::const_iterator index = _videoOutputMap.find(videoOutput);

        if (index != _videoOutputMap.end()) {
            defaultResolution = index->second.DefaultResolution;
            result = Core::ERROR_NONE;
        }

        return (result);
    }

    Core::hresult DeviceInfoImplementation::Resolutions(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, Exchange::IDeviceVideoCapabilities::IScreenResolutionIterator*& resolutions) const
    {
        Core::hresult result = Core::ERROR_UNKNOWN_KEY;

        VideoOutputMap::const_iterator index = _videoOutputMap.find(videoOutput);

        if ((index != _videoOutputMap.end())) {

            if (index->second.Resolutions.size() > 0) {
                resolutions = Core::ServiceType<ResolutionIteratorImplementation>::Create<Exchange::IDeviceVideoCapabilities::IScreenResolutionIterator>(index->second.Resolutions);
                result = Core::ERROR_NONE;
            }
            else {
                result = Core::ERROR_GENERAL;
            }
        }

        return (result);
    }

    Core::hresult DeviceInfoImplementation::Hdcp(const VideoOutput videoOutput, CopyProtection& hdcpVersion) const
    {
        Core::hresult result = Core::ERROR_UNKNOWN_KEY;

        VideoOutputMap::const_iterator index = _videoOutputMap.find(videoOutput);

        if (index != _videoOutputMap.end()) {
            hdcpVersion = index->second.CopyProtection;
            result = Core::ERROR_NONE;
        }

        return (result);
    }

    Core::hresult DeviceInfoImplementation::HDR(bool& supportsHDR) const
    {
        supportsHDR = _supportsHdr;
        return Core::ERROR_NONE;
    }

    Core::hresult DeviceInfoImplementation::Atmos(bool& supportsAtmos) const
    {
        supportsAtmos = _supportsAtmos;
        return Core::ERROR_NONE;
    }

    Core::hresult DeviceInfoImplementation::CEC(bool& supportsCEC) const
    {
        supportsCEC = _supportsCEC;
        return Core::ERROR_NONE;
    }

    Core::hresult DeviceInfoImplementation::HostEDID(string& edid) const
    {
        edid = _hostEdid;
        return Core::ERROR_NONE;
    }

    Core::hresult DeviceInfoImplementation::Make(string& value) const
    {
        Core::hresult result = Core::ERROR_UNAVAILABLE;
        if (_make.empty() == false) {
            value = _make;
            result = Core::ERROR_NONE;
        }
        return result;
    }

    Core::hresult DeviceInfoImplementation::SerialNumber(string& value) const
    {
        uint32_t result = Core::ERROR_UNAVAILABLE;
        if (_serialNumber.empty() == false) {
            value = _serialNumber;
            result = Core::ERROR_NONE;
        }
        return result;
    }

    Core::hresult DeviceInfoImplementation::ModelID(string& value) const
    {
        uint32_t result = Core::ERROR_UNAVAILABLE;
        if (_sku.empty() == false) {
            value = _sku;
            result = Core::ERROR_NONE;
        }
        return result;
    }

    Core::hresult DeviceInfoImplementation::ModelName(string& value) const
    {
        Core::hresult result = Core::ERROR_UNAVAILABLE;
        if (_modelName.empty() == false) {
            value = _modelName;
            result = Core::ERROR_NONE;
        }
        return result;
    }

    Core::hresult DeviceInfoImplementation::ModelYear(uint16_t& value) const
    {
        Core::hresult result = Core::ERROR_UNAVAILABLE;
        if (_modelYear != 0) {
            value = _modelYear;
            result = Core::ERROR_NONE;
        }
        return result;
    }

    Core::hresult DeviceInfoImplementation::FriendlyName(string& value) const
    {
        Core::hresult result = Core::ERROR_UNAVAILABLE;
        if (_friendlyName.empty() == false) {
            value = _friendlyName;
            result = Core::ERROR_NONE;
        }
        return result;
    }

    Core::hresult DeviceInfoImplementation::DeviceType(Exchange::IDeviceInfo::Type& value) const
    {
        Core::hresult result = Core::ERROR_UNAVAILABLE;

        if (_deviceType.empty() == false) {

            Core::EnumerateType<Exchange::IDeviceInfo::Type> deviceTypeEnum(_deviceType.c_str(), false);

            if (deviceTypeEnum.IsSet()) {
                value = deviceTypeEnum.Value();
                result = Core::ERROR_NONE;
            }
            else {
                TRACE(Trace::Fatal, (_T("Unknown value: %s"), _deviceType.c_str()));
                result = Core::ERROR_UNKNOWN_KEY;
            }
        }
        return (result);
    }

    Core::hresult DeviceInfoImplementation::DistributorID(string& value) const
    {
        Core::hresult result = Core::ERROR_UNAVAILABLE;
        if (_distributorId.empty() == false) {
            value = _distributorId;
            result = Core::ERROR_NONE;
        }
        return result;
    }

    Core::hresult DeviceInfoImplementation::PlatformName(string& value) const
    {
        Core::hresult result = Core::ERROR_UNAVAILABLE;
        if (_platformName.empty() == false) {
            value = _platformName;
            result = Core::ERROR_NONE;
        }
        return result;
    }

    Core::hresult DeviceInfoImplementation::Firmware(Exchange::IDeviceInfo::FirmwareInfo& /* value */) const {
        return (Core::ERROR_UNAVAILABLE);
    }
}
}
