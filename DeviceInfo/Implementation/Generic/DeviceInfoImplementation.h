#pragma once

#include "../../Module.h"
#include <interfaces/IDeviceInfo.h>

namespace WPEFramework {
namespace Plugin {
    class DeviceInfoImplementation : public Exchange::IDeviceInfo,
                                     public Exchange::IDeviceAudioCapabilities,
                                     public Exchange::IDeviceVideoCapabilities {
    private:
        class Config : public Core::JSON::Container {
            using AudioCapabilitiesJsonArray = Core::JSON::ArrayType<Core::JSON::EnumType<Exchange::IDeviceAudioCapabilities::AudioCapability>>;
            using MS12CapabilitiesJsonArray = Core::JSON::ArrayType<Core::JSON::EnumType<Exchange::IDeviceAudioCapabilities::MS12Capability>>;
            using MS12ProfileJsonArray = Core::JSON::ArrayType<Core::JSON::EnumType<Exchange::IDeviceAudioCapabilities::MS12Profile>>;
            using AudioOutputJson = Core::JSON::EnumType<Exchange::IDeviceAudioCapabilities::AudioOutput>;

        public:
            class AudioOutput : public Core::JSON::Container {
            public:
                AudioOutput& operator=(const AudioOutput&) = delete;
                AudioOutput()
                    : Core::JSON::Container()
                    , Name()
                    , AudioCapabilities()
                    , MS12Capabilities()
                    , MS12Profiles()
                {
                    Add(_T("name"), &Name);
                    Add(_T("audiocapabilities"), &AudioCapabilities);
                    Add(_T("ms12capabilities"), &MS12Capabilities);
                    Add(_T("ms12profiles"), &MS12Profiles);
                }
                AudioOutput(const AudioOutput& copy)
                    : Core::JSON::Container()
                    , Name(copy.Name)
                    , AudioCapabilities(copy.AudioCapabilities)
                    , MS12Capabilities(copy.MS12Capabilities)
                    , MS12Profiles(copy.MS12Profiles)
                {
                    Add(_T("name"), &Name);
                    Add(_T("audiocapabilities"), &AudioCapabilities);
                    Add(_T("ms12capabilities"), &MS12Capabilities);
                    Add(_T("ms12profiles"), &MS12Profiles);
                }
                ~AudioOutput() = default;

                AudioOutputJson Name;
                AudioCapabilitiesJsonArray AudioCapabilities;
                MS12CapabilitiesJsonArray MS12Capabilities;
                MS12ProfileJsonArray MS12Profiles;
            };

            using ResolutionsJsonArray = Core::JSON::ArrayType<Core::JSON::EnumType<Exchange::IDeviceVideoCapabilities::ScreenResolution>>;
            using VideoOutputJson = Core::JSON::EnumType<Exchange::IDeviceVideoCapabilities::VideoOutput>;
            class VideoOutput : public Core::JSON::Container {
            public:
                VideoOutput& operator=(const VideoOutput&) = delete;
                VideoOutput()
                    : Core::JSON::Container()
                    , Name()
                    , Resolutions()
                    , DefaultResolution(Exchange::IDeviceVideoCapabilities::ScreenResolution_Unknown)
                    , CopyProtection(Exchange::IDeviceVideoCapabilities::CopyProtection::HDCP_UNAVAILABLE)
                {
                    Add(_T("name"), &Name);
                    Add(_T("resolutions"), &Resolutions);
                    Add(_T("defaultresolution"), &DefaultResolution);
                    Add(_T("copyprotection"), &CopyProtection);
                }
                VideoOutput(const VideoOutput& copy)
                    : Core::JSON::Container()
                    , Name(copy.Name)
                    , Resolutions(copy.Resolutions)
                    , DefaultResolution(copy.DefaultResolution)
                    , CopyProtection(copy.CopyProtection)
                {
                    Add(_T("name"), &Name);
                    Add(_T("resolutions"), &Resolutions);
                    Add(_T("defaultresolution"), &DefaultResolution);
                    Add(_T("copyprotection"), &CopyProtection);
                }

                ~VideoOutput() = default;

                VideoOutputJson Name;
                ResolutionsJsonArray Resolutions;
                Core::JSON::EnumType<Exchange::IDeviceVideoCapabilities::ScreenResolution> DefaultResolution;
                Core::JSON::EnumType<Exchange::IDeviceVideoCapabilities::CopyProtection> CopyProtection;
            };

        public:
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;
            Config()
                : Core::JSON::Container()
                , Hdr(false)
                , Atmos(false)
                , Cec(false)
                , Edid()
                , AudioOutputs()
                , VideoOutputs()
                , Make()
                , ModelName()
                , ModelYear()
                , FriendlyName()
                , DeviceType()
                , DistributorId()
                , SerialNumber()
                , Sku()
                , PlatformName()
            {
                Add(_T("hdr"), &Hdr);
                Add(_T("atmos"), &Atmos);
                Add(_T("cec"), &Cec);
                Add(_T("edid"), &Edid);
                Add(_T("audiooutputs"), &AudioOutputs);
                Add(_T("videooutputs"), &VideoOutputs);
                Add(_T("make"), &Make);
                Add(_T("modelname"), &ModelName);
                Add(_T("modelyear"), &ModelYear);
                Add(_T("friendlyname"), &FriendlyName);
                Add(_T("devicetype"), &DeviceType);
                Add(_T("distributorid"), &DistributorId);
                Add(_T("serialnumber"), &SerialNumber);
                Add(_T("sku"), &Sku);
                Add(_T("platformname"), &PlatformName);
            }
            ~Config() = default;

            Core::JSON::Boolean Hdr;
            Core::JSON::Boolean Atmos;
            Core::JSON::Boolean Cec;
            Core::JSON::String Edid;
            Core::JSON::ArrayType<AudioOutput> AudioOutputs;
            Core::JSON::ArrayType<VideoOutput> VideoOutputs;
            Core::JSON::String Make;
            Core::JSON::String ModelName;
            Core::JSON::DecUInt16 ModelYear;
            Core::JSON::String FriendlyName;
            Core::JSON::String DeviceType;
            Core::JSON::String DistributorId;
            Core::JSON::String SerialNumber;
            Core::JSON::String Sku;
            Core::JSON::String PlatformName;
        }; // CONFIG

        struct AudioOutputCapability {
            std::list<IDeviceAudioCapabilities::AudioCapability> AudioCapabilities;
            std::list<IDeviceAudioCapabilities::MS12Capability> MS12Capabilities;
            std::list<IDeviceAudioCapabilities::MS12Profile> MS12Profiles;
        };
        using AudioOutputMap = std::map<Exchange::IDeviceAudioCapabilities::AudioOutput, AudioOutputCapability>;
        using AudioOutputList = std::list<Exchange::IDeviceAudioCapabilities::AudioOutput>;
        using AudioOutputIteratorImplementation = RPC::IteratorType<Exchange::IDeviceAudioCapabilities::IAudioOutputIterator>;

        struct VideoOutputCapability {
            std::list<IDeviceVideoCapabilities::ScreenResolution> Resolutions;
            IDeviceVideoCapabilities::ScreenResolution DefaultResolution;
            IDeviceVideoCapabilities::CopyProtection CopyProtection;
        };
        using VideoOutputMap = std::map<Exchange::IDeviceVideoCapabilities::VideoOutput, VideoOutputCapability>;
        using VideoOutputList = std::list<Exchange::IDeviceVideoCapabilities::VideoOutput>;
        using VideoOutputIteratorImplementation = RPC::IteratorType<Exchange::IDeviceVideoCapabilities::IVideoOutputIterator>;

    public:
        DeviceInfoImplementation(const DeviceInfoImplementation&) = delete;
        DeviceInfoImplementation& operator=(const DeviceInfoImplementation&) = delete;
        DeviceInfoImplementation()
            : _config()
            , _supportsHdr(false)
            , _supportsAtmos(false)
            , _supportsCEC(false)
            , _hostEdid()
            , _audioOutputMap()
            , _videoOutputMap()
            , _make()
            , _modelName()
            , _modelYear()
            , _friendlyName()
            , _deviceType()
            , _distributorId()
            , _serialNumber()
            , _sku()
            , _platformName()
        {
        }
        ~DeviceInfoImplementation() {}

        BEGIN_INTERFACE_MAP(DeviceInfoImplementation)
        INTERFACE_ENTRY(Exchange::IDeviceInfo)
        INTERFACE_ENTRY(Exchange::IDeviceAudioCapabilities)
        INTERFACE_ENTRY(Exchange::IDeviceVideoCapabilities)
        END_INTERFACE_MAP

        uint32_t Configure(const PluginHost::IShell* service) override;

        uint32_t AudioOutputs(IAudioOutputIterator*& res) const override;
        uint32_t AudioCapabilities(const AudioOutput audioOutput, IAudioCapabilityIterator*& audioCapabilities) const override;
        uint32_t MS12Capabilities(const AudioOutput audioOutput, IMS12CapabilityIterator*& ms12Capabilities) const override;
        uint32_t MS12AudioProfiles(const AudioOutput audioOutput, IMS12ProfileIterator*& ms12AudioProfiles) const override;

        uint32_t VideoOutputs(IVideoOutputIterator*& videoOutputs) const override;
        uint32_t DefaultResolution(const VideoOutput videoOutput, ScreenResolution& defaultResolution) const override;
        uint32_t Resolutions(const VideoOutput videoOutput, IScreenResolutionIterator*& resolutions) const override;
        uint32_t Hdcp(const VideoOutput videoOutput, CopyProtection& hdcpVersion) const override;
        uint32_t HDR(bool& supportsHDR) const override;
        uint32_t Atmos(bool& supportsAtmos) const override;
        uint32_t CEC(bool& supportsCEC) const override;
        uint32_t HostEDID(string& edid) const override;

        uint32_t Make(string& value) const override;
        uint32_t ModelName(string& value) const override;
        uint32_t ModelYear(uint16_t& value) const override;
        uint32_t DeviceType(string& value) const override;
        uint32_t FriendlyName(string& value) const override;
        uint32_t DistributorId(string& value) const override;
        uint32_t SerialNumber(string& value) const override;
        uint32_t Sku(string& value) const override;
        uint32_t PlatformName(string& value) const override;

    private:
        Config _config;
        bool _supportsHdr;
        bool _supportsAtmos;
        bool _supportsCEC;
        string _hostEdid;
        AudioOutputMap _audioOutputMap;
        VideoOutputMap _videoOutputMap;
        string _make;
        string _modelName;
        uint16_t _modelYear;
        string _friendlyName;
        string _deviceType;
        string _distributorId;
        string _serialNumber;
        string _sku;
        string _platformName;
    };
} //namespace WPEFramework
} //namespace Plugin
