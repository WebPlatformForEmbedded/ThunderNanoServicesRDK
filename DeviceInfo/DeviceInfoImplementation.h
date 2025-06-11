#pragma once

#include "Module.h"
#include <interfaces/IConfiguration.h>
#include <interfaces/IDeviceInfo.h>

namespace Thunder {
namespace Plugin {
    class DeviceInfoImplementation : public Exchange::IDeviceInfo,
                                     public Exchange::IDeviceAudioCapabilities,
                                     public Exchange::IDeviceVideoCapabilities,
                                     public Exchange::IConfiguration {
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
                    , CopyProtection()
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
        INTERFACE_ENTRY(Exchange::IConfiguration)
        END_INTERFACE_MAP

        uint32_t Configure(PluginHost::IShell* service) override;

        Core::hresult SerialNumber(string& value) const override;
        Core::hresult ModelID(string& value) const override;
        Core::hresult Make(string& value) const override;
        Core::hresult ModelName(string& value) const override;
        Core::hresult ModelYear(uint16_t& value) const override;
        Core::hresult FriendlyName(string& value) const override;
        Core::hresult DeviceType(Exchange::IDeviceInfo::Type& value) const override;
        Core::hresult PlatformName(string& value) const override;
        Core::hresult DistributorID(string& value) const override;
        Core::hresult Firmware(Exchange::IDeviceInfo::FirmwareInfo& value) const override;

        Core::hresult AudioOutputs(Exchange::IDeviceAudioCapabilities::IAudioOutputIterator*& audioOutputs) const override;
        Core::hresult AudioCapabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator*& audioCapabilities) const override;
        Core::hresult MS12Capabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IMS12CapabilityIterator*& ms12Capabilities) const override;
        Core::hresult MS12AudioProfiles(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IMS12ProfileIterator*& ms12Profiles) const override;

        Core::hresult VideoOutputs(Exchange::IDeviceVideoCapabilities::IVideoOutputIterator*& videoOutputs) const override;
        Core::hresult DefaultResolution(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, Exchange::IDeviceVideoCapabilities::ScreenResolution& defaultResolution) const override;
        Core::hresult Resolutions(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, Exchange::IDeviceVideoCapabilities::IScreenResolutionIterator*& resolutions) const override;
        Core::hresult Hdcp(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, Exchange::IDeviceVideoCapabilities::CopyProtection& hdcpVersion) const override;
        Core::hresult HostEDID(string& edid) const override;
        Core::hresult HDR(bool& supportsHDR) const override;
        Core::hresult Atmos(bool& supportsAtmos) const override;
        Core::hresult CEC(bool& supportsCEC) const override;

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
} //namespace Thunder
} //namespace Plugin
