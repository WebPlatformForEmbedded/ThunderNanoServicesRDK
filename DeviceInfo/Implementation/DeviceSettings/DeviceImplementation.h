#pragma once

#include "../../Module.h"
#ifdef USE_THUNDER_R4
#include <interfaces/IDeviceInfo.h>
#else
#include <interfaces/IDeviceInfo2.h>
#endif /* USE_THUNDER_R4 */
#include <interfaces/IFirmwareVersion.h>

namespace WPEFramework {
namespace Plugin {
    class DeviceInfoImplementation : public Exchange::IDeviceInfo,
                                     public Exchange::IDeviceAudioCapabilities,
                                     public Exchange::IDeviceVideoCapabilities,
                                     public Exchange::IFirmwareVersion {

    private:
        DeviceInfoImplementation(const DeviceInfoImplementation&) = delete;
        DeviceInfoImplementation& operator=(const DeviceInfoImplementation&) = delete;

    public:
        DeviceInfoImplementation();

        BEGIN_INTERFACE_MAP(DeviceInfoImplementation)
        INTERFACE_ENTRY(Exchange::IDeviceInfo)
        INTERFACE_ENTRY(Exchange::IDeviceAudioCapabilities)
        INTERFACE_ENTRY(Exchange::IDeviceVideoCapabilities)
        INTERFACE_ENTRY(Exchange::IFirmwareVersion)
        END_INTERFACE_MAP

    private:
        // IDeviceInfo interface
        uint32_t SerialNumber(string& serialNumber) const override;
        uint32_t Sku(string& sku) const override;
        uint32_t Make(string& make) const override;
        uint32_t Model(string& model) const override;
        uint32_t DeviceType(string& deviceType) const override;
        uint32_t DistributorId(string& distributorId) const override;

        uint32_t SupportedAudioPorts(RPC::IStringIterator*& supportedAudioPorts) const override;
        uint32_t AudioCapabilities(const string& audioPort, Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator*& audioCapabilities) const override;
        uint32_t MS12Capabilities(const string& audioPort, Exchange::IDeviceAudioCapabilities::IMS12CapabilityIterator*& ms12Capabilities) const override;
        uint32_t SupportedMS12AudioProfiles(const string& audioPort, RPC::IStringIterator*& supportedMS12AudioProfiles) const override;

        // IDeviceVideoCapabilities interface
        uint32_t SupportedVideoDisplays(RPC::IStringIterator*& supportedVideoDisplays) const override;
        uint32_t HostEDID(string& edid) const override;
        uint32_t DefaultResolution(const string& videoDisplay, string& defaultResolution) const override;
        uint32_t SupportedResolutions(const string& videoDisplay, RPC::IStringIterator*& supportedResolutions) const override;
        uint32_t SupportedHdcp(const string& videoDisplay, Exchange::IDeviceVideoCapabilities::CopyProtection& supportedHDCPVersion) const override;

        // IFirmwareVersion interface
        uint32_t Imagename(string& imagename) const override;
        uint32_t Sdk(string& sdk) const override;
        uint32_t Mediarite(string& mediarite) const override;
        uint32_t Yocto(string& yocto) const override;
    };
}
}
