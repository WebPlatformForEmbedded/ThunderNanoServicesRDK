<!-- Generated automatically, DO NOT EDIT! -->
<a id="head_Device_Info_Plugin"></a>
# Device Info Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

DeviceInfo plugin for Thunder framework.

### Table of Contents

- [Introduction](#head_Introduction)
- [Description](#head_Description)
- [Configuration](#head_Configuration)
- [Interfaces](#head_Interfaces)
- [Methods](#head_Methods)
- [Properties](#head_Properties)

<a id="head_Introduction"></a>
# Introduction

<a id="head_Scope"></a>
## Scope

This document describes purpose and functionality of the DeviceInfo plugin. It includes detailed specification about its configuration, methods and properties provided.

<a id="head_Case_Sensitivity"></a>
## Case Sensitivity

All identifiers of the interfaces described in this document are case-sensitive. Thus, unless stated otherwise, all keywords, entities, properties, relations and actions should be treated as such.

<a id="head_Acronyms,_Abbreviations_and_Terms"></a>
## Acronyms, Abbreviations and Terms

The table below provides and overview of acronyms used in this document and their definitions.

| Acronym | Description |
| :-------- | :-------- |
| <a name="acronym.API">API</a> | Application Programming Interface |
| <a name="acronym.HTTP">HTTP</a> | Hypertext Transfer Protocol |
| <a name="acronym.JSON">JSON</a> | JavaScript Object Notation; a data interchange format |
| <a name="acronym.JSON-RPC">JSON-RPC</a> | A remote procedure call protocol encoded in JSON |

The table below provides and overview of terms and abbreviations used in this document and their definitions.

| Term | Description |
| :-------- | :-------- |
| <a name="term.callsign">callsign</a> | The name given to an instance of a plugin. One plugin can be instantiated multiple times, but each instance the instance name, callsign, must be unique. |

<a id="head_References"></a>
## References

| Ref ID | Description |
| :-------- | :-------- |
| <a name="ref.HTTP">[HTTP](http://www.w3.org/Protocols)</a> | HTTP specification |
| <a name="ref.JSON-RPC">[JSON-RPC](https://www.jsonrpc.org/specification)</a> | JSON-RPC 2.0 specification |
| <a name="ref.JSON">[JSON](http://www.json.org/)</a> | JSON specification |
| <a name="ref.Thunder">[Thunder](https://github.com/WebPlatformForEmbedded/Thunder/blob/master/doc/WPE%20-%20API%20-%20Thunder.docx)</a> | Thunder API Reference |

<a id="head_Description"></a>
# Description

The DeviceInfo plugin allows retrieving of various device-related information.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a id="head_Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *DeviceInfo*) |
| classname | string | mandatory | Class name: *DeviceInfo* |
| locator | string | mandatory | Library name: *libThunderDeviceInfo.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |
| hdr | boolean | mandatory | Does the device support HDR (true or false) |
| atmos | boolean | mandatory | Does the device support Dolby Atmos (true or false) |
| cec | boolean | mandatory | Does the device support HDMI CEC (true or false) |
| hdcp | string | mandatory | HDCP version supported by the device (unavailable, hdcp_14, hdcp_20, hdcp_21, hdcp_22) |
| audio | array | mandatory | Supported audio outputs |
| audio[#] | string | mandatory | Audio output (other, rf_modulator, analog, spdif, hdmi, displayport) |
| video | array | mandatory | Supported video outputs |
| video[#] | string | mandatory | Video output (other, rf_modulator, composite, svideo, component, scart_rgb, hdmi, displayport) |
| resolution | array | mandatory | Supported output resolutions |
| resolution[#] | string | mandatory | Output resolution (unknown, 480i, 480p, 576i, 576p, 720p, 1080i, 1080p, 2160p30, 2160p60, 4320p30, 4320p60) |
| modelName | string | mandatory | Model Name |
| modelYear | integer | mandatory | Model Year |
| friendlyName | string | mandatory | Friendly Name |
| systemIntegratorName | string | mandatory | System Integrator Name |
| platformName | string | mandatory | Platform Name |

<a id="head_Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- IDeviceInfo ([IDeviceInfo.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IDeviceInfo.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- IDeviceAudioCapabilities ([IDeviceInfo.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IDeviceInfo.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- IDeviceVideoCapabilities ([IDeviceInfo.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IDeviceInfo.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- ISystemInfo ([IDeviceInfo.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IDeviceInfo.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a id="head_Methods"></a>
# Methods

The following methods are provided by the DeviceInfo plugin:

DeviceVideoCapabilities interface methods:

| Method | Description |
| :-------- | :-------- |
| [hdr](#method_hdr) | HDR support by this device |

<a id="method_hdr"></a>
## *hdr [<sup>method</sup>](#head_Methods)*

HDR support by this device.

### Parameters

This method takes no parameters.

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | boolean | mandatory | True if device supports HDR, false otherwise |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.hdr"
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": false
}
```

<a id="head_Properties"></a>
# Properties

The following properties are provided by the DeviceInfo plugin:

DeviceInfo interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [serialnumber](#property_serialnumber) | read-only | Device serial number |
| [modelid](#property_modelid) | read-only | Device model number or stock keeping unit (SKU) |
| [make](#property_make) | read-only | Device manufacturer |
| [modelname](#property_modelname) | read-only | Device model name |
| [modelyear](#property_modelyear) | read-only | Device model year |
| [friendlyname](#property_friendlyname) | read-only | Device friendly name |
| [devicetype](#property_devicetype) | read-only | Device type |
| [platformname](#property_platformname) | read-only | Device platform name |
| [distributorid](#property_distributorid) | read-only | Partner ID or distributor ID for device |
| [firmware](#property_firmware) / [firmwareversion](#property_firmware) | read-only | Versions maintained in version |

DeviceAudioCapabilities interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [audiooutputs](#property_audiooutputs) / [supportedaudioports](#property_audiooutputs) | read-only | Audio ports supported on the device (all ports that are physically present |
| [audiocapabilities](#property_audiocapabilities) | read-only | Audio capabilities for the specified audio port |
| [ms12capabilities](#property_ms12capabilities) | read-only | Audio ms12 capabilities for the specified audio port |
| [ms12audioprofiles](#property_ms12audioprofiles) / [supportedms12audioprofiles](#property_ms12audioprofiles) | read-only | Audio ms12 profiles for the specified audio port |

DeviceVideoCapabilities interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [videooutputs](#property_videooutputs) / [supportedvideoports](#property_videooutputs) | read-only | Video ports supported on the device (all ports that are physically present) |
| [defaultresolution](#property_defaultresolution) | read-only | Default resolution for the specified video output |
| [resolutions](#property_resolutions) / [supportedresolutions](#property_resolutions) | read-only | Resolutions supported by the specified video output |
| [hdcp](#property_hdcp) / [supportedhdcp](#property_hdcp) | read-only | High-bandwidth Digital Content Protection (HDCP) supported by the specified video output |
| [hostedid](#property_hostedid) | read-only | Extended Display Identification Data (EDID) of the host |
| [atmos](#property_atmos) | read-only | Atmos support by this device |
| [cec](#property_cec) | read-only | CEC support by this device |

SystemInfo interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [version](#property_version) | read-only | Software version (in form *version#hashtag*) param value: Software version of the device  |
| [uptime](#property_uptime) | read-only | System uptime (in seconds) param value: System uptime in seconds  |
| [totalram](#property_totalram) | read-only | Total installed system RAM (in bytes) param value: Total installed system RAM in bytes  |
| [freeram](#property_freeram) | read-only | Free system RAM (in bytes) param value: System RAM still available in bytes  |
| [totalswap](#property_totalswap) | read-only | Total swap space (in bytes) param value: Total swap space in bytes  |
| [freeswap](#property_freeswap) | read-only | Free swap space (in bytes) param value: Swap space still available in bytes  |
| [devicename](#property_devicename) | read-only | Host name param value: Host name of the device  |
| [cpuload](#property_cpuload) | read-only | Current CPU load (percentage) param value: Current CPU load in percentage  |
| [cpuloadavgs](#property_cpuloadavgs) | read-only | CPU load averages from 1, 5 and 15 minutes (percentage) param value: An array of three CPU load averages in percentage  |
| [addresses](#property_addresses) | read-only | Network interface addresses |
| [socketinfo](#property_socketinfo) | read-only | Socket information |

<a id="property_serialnumber"></a>
## *serialnumber [<sup>property</sup>](#head_Properties)*

Provides access to the device serial number.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Serial number set by manufacturer |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNAVAILABLE``` | Information about the serial number of the device is not available |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.serialnumber"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "PAW400003744"
}
```

<a id="property_modelid"></a>
## *modelid [<sup>property</sup>](#head_Properties)*

Provides access to the device model number or stock keeping unit (SKU).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Model number or SKU of the device |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNAVAILABLE``` | Information about the model number or SKU of the device is not available |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.modelid"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "PX051AEI"
}
```

<a id="property_make"></a>
## *make [<sup>property</sup>](#head_Properties)*

Provides access to the device manufacturer.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Manufacturer of the device |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNAVAILABLE``` | Information about the manufacturer of the device is not available |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.make"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "pace"
}
```

<a id="property_modelname"></a>
## *modelname [<sup>property</sup>](#head_Properties)*

Provides access to the device model name.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Model name of the device |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.modelname"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "Pace Xi5"
}
```

<a id="property_modelyear"></a>
## *modelyear [<sup>property</sup>](#head_Properties)*

Provides access to the device model year.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Year of the model |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNAVAILABLE``` | Information about the model year of the device is not available |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.modelyear"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": 2020
}
```

<a id="property_friendlyname"></a>
## *friendlyname [<sup>property</sup>](#head_Properties)*

Provides access to the device friendly name.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Friendly name of the device |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNAVAILABLE``` | Information about the friendly name of the device is not available |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.friendlyname"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "My Device"
}
```

<a id="property_devicetype"></a>
## *devicetype [<sup>property</sup>](#head_Properties)*

Provides access to the device type.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Type of the device (must be one of the following: *Hybrid, IpStb, MediaClient, QamIpStb, Tv*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNAVAILABLE``` | Information about a type of the device is not available |
| ```ERROR_UNKNOWN_KEY``` | The device type is not recognized |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.devicetype"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "IP_STB"
}
```

<a id="property_platformname"></a>
## *platformname [<sup>property</sup>](#head_Properties)*

Provides access to the device platform name.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Platform name of the device |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNAVAILABLE``` | Information about the platform name of the device is not available |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.platformname"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "Linux"
}
```

<a id="property_distributorid"></a>
## *distributorid [<sup>property</sup>](#head_Properties)*

Provides access to the partner ID or distributor ID for device.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Partner ID or distributor ID for device |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNAVAILABLE``` | Information about the distributor ID of the device is not available |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.distributorid"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "Comcast"
}
```

<a id="property_firmware"></a>
## *firmware [<sup>property</sup>](#head_Properties)*

Provides access to the versions maintained in version.txt.

> This property is **read-only**.

> ``firmwareversion`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | object | mandatory | Name of the firmware image and its specific versions |
| (property).imagename | string | mandatory | Name of firmware image |
| (property)?.sdk | string | optional | SDK version string |
| (property)?.mediarite | string | optional | Mediarite value |
| (property)?.build | string | optional | Yocto version |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNAVAILABLE``` | Information about the firmware version is not available |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.firmware"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "imagename": "PX051AEI_VBN_2203_sprint_20220331225312sdy_NG",
    "sdk": "17.3",
    "mediarite": "8.3.53",
    "build": "dunfell"
  }
}
```

<a id="property_audiooutputs"></a>
## *audiooutputs [<sup>property</sup>](#head_Properties)*

Provides access to the audio ports supported on the device (all ports that are physically present.

> This property is **read-only**.

> ``supportedaudioports`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | An array of audio outputs supported by the device |
| (property)[#] | string | mandatory | *...* (must be one of the following: *Analog, DisplayPort, Hdmi0, Hdmi1, Other, RfModulator, Spdif*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | General error |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.audiooutputs"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "RfModulator"
  ]
}
```

<a id="property_audiocapabilities"></a>
## *audiocapabilities [<sup>property</sup>](#head_Properties)*

Provides access to the audio capabilities for the specified audio port.

> This property is **read-only**.

> The *audiooutput* parameter shall be passed as the index to the property, i.e. ``audiocapabilities@<audiooutput>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| audiooutput | string | mandatory | Audio output supported by the device (must be one of the following: *Analog, DisplayPort, Hdmi0, Hdmi1, Other, RfModulator, Spdif*) |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | An array of audio capabilities supported by specified port |
| (property)[#] | string | mandatory | *...* (must be one of the following: *Atmos, Dad, DapV2, Dd, DdPlus, Ms12*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | General error |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.audiocapabilities@xyz"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "Dd"
  ]
}
```

<a id="property_ms12capabilities"></a>
## *ms12capabilities [<sup>property</sup>](#head_Properties)*

Provides access to the audio ms12 capabilities for the specified audio port.

> This property is **read-only**.

> The *audiooutput* parameter shall be passed as the index to the property, i.e. ``ms12capabilities@<audiooutput>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| audiooutput | string | mandatory | Audio output supported by the device (must be one of the following: *Analog, DisplayPort, Hdmi0, Hdmi1, Other, RfModulator, Spdif*) |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | An array of MS12 audio capabilities supported by specified port |
| (property)[#] | string | mandatory | *...* (must be one of the following: *DialogueEnhancer, DolbyVolume, InteligentEqualizer*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | General error |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.ms12capabilities@xyz"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "InteligentEqualizer"
  ]
}
```

<a id="property_ms12audioprofiles"></a>
## *ms12audioprofiles [<sup>property</sup>](#head_Properties)*

Provides access to the audio ms12 profiles for the specified audio port.

> This property is **read-only**.

> ``supportedms12audioprofiles`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

> The *audiooutput* parameter shall be passed as the index to the property, i.e. ``ms12audioprofiles@<audiooutput>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| audiooutput | string | mandatory | Audio output supported by the device (must be one of the following: *Analog, DisplayPort, Hdmi0, Hdmi1, Other, RfModulator, Spdif*) |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | An array of MS12 audio profiles supported by specified port |
| (property)[#] | string | mandatory | *...* (must be one of the following: *Movie, Music, Voice*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | General error |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.ms12audioprofiles@xyz"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "Movie"
  ]
}
```

<a id="property_videooutputs"></a>
## *videooutputs [<sup>property</sup>](#head_Properties)*

Provides access to the video ports supported on the device (all ports that are physically present).

> This property is **read-only**.

> ``supportedvideoports`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | An array of video outputs supported by the device |
| (property)[#] | string | mandatory | *...* (must be one of the following: *Component, Composite, DisplayPort, Hdmi0, Hdmi1, Other, RfModulator, SVideo, ScartRgb*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | General error |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.videooutputs"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "RfModulator"
  ]
}
```

<a id="property_defaultresolution"></a>
## *defaultresolution [<sup>property</sup>](#head_Properties)*

Provides access to the default resolution for the specified video output.

> This property is **read-only**.

> The *videooutput* parameter shall be passed as the index to the property, i.e. ``defaultresolution@<videooutput>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| videooutput | string | mandatory | Video output supported by the device (must be one of the following: *Component, Composite, DisplayPort, Hdmi0, Hdmi1, Other, RfModulator, SVideo, ScartRgb*) |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Default resolution supported by specified port (must be one of the following: *ScreenResolution1080i, ScreenResolution1080i25hz, ScreenResolution1080i50hz, ScreenResolution1080p, ScreenResolution1080p24hz, ScreenResolution1080p25hz, ScreenResolution1080p30hz, ScreenResolution1080p50hz, ScreenResolution1080p60hz, ScreenResolution2160p30hz, ScreenResolution2160p50hz, ScreenResolution2160p60hz, ScreenResolution4320p30hz, ScreenResolution4320p60hz, ScreenResolution480i, ScreenResolution480p, ScreenResolution576i, ScreenResolution576p, ScreenResolution576p50hz, ScreenResolution720p, ScreenResolution720p50hz, ScreenResolutionUnknown*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNKNOWN_KEY``` | Provided video output is not recognized |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.defaultresolution@xyz"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "ScreenResolution480i"
}
```

<a id="property_resolutions"></a>
## *resolutions [<sup>property</sup>](#head_Properties)*

Provides access to the resolutions supported by the specified video output.

> This property is **read-only**.

> ``supportedresolutions`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

> The *videooutput* parameter shall be passed as the index to the property, i.e. ``resolutions@<videooutput>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| videooutput | string | mandatory | Video output supported by the device (must be one of the following: *Component, Composite, DisplayPort, Hdmi0, Hdmi1, Other, RfModulator, SVideo, ScartRgb*) |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | An array of resolutions supported by specified port |
| (property)[#] | string | mandatory | *...* (must be one of the following: *ScreenResolution1080i, ScreenResolution1080i25hz, ScreenResolution1080i50hz, ScreenResolution1080p, ScreenResolution1080p24hz, ScreenResolution1080p25hz, ScreenResolution1080p30hz, ScreenResolution1080p50hz, ScreenResolution1080p60hz, ScreenResolution2160p30hz, ScreenResolution2160p50hz, ScreenResolution2160p60hz, ScreenResolution4320p30hz, ScreenResolution4320p60hz, ScreenResolution480i, ScreenResolution480p, ScreenResolution576i, ScreenResolution576p, ScreenResolution576p50hz, ScreenResolution720p, ScreenResolution720p50hz, ScreenResolutionUnknown*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNKNOWN_KEY``` | Provided video output is not recognized |
| ```ERROR_GENERAL``` | General error |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.resolutions@xyz"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "ScreenResolution480i"
  ]
}
```

<a id="property_hdcp"></a>
## *hdcp [<sup>property</sup>](#head_Properties)*

Provides access to the high-bandwidth Digital Content Protection (HDCP) supported by the specified video output.

> This property is **read-only**.

> ``supportedhdcp`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

> The *videooutput* parameter shall be passed as the index to the property, i.e. ``hdcp@<videooutput>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| videooutput | string | mandatory | Video output supported by the device (must be one of the following: *Component, Composite, DisplayPort, Hdmi0, Hdmi1, Other, RfModulator, SVideo, ScartRgb*) |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | HDCP version supported by specified port (must be one of the following: *Hdcp14, Hdcp20, Hdcp21, Hdcp22*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNKNOWN_KEY``` | Provided video output is not recognized |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.hdcp@xyz"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "Hdcp20"
}
```

<a id="property_hostedid"></a>
## *hostedid [<sup>property</sup>](#head_Properties)*

Provides access to the extended Display Identification Data (EDID) of the host.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | A base64 encoded byte array string representing the EDID of the host |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.hostedid"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "AP///////wAQrMLQVEJTMQUdAQOANR546q11qVRNnSYPUFSlSwCBALMA0QBxT6lAgYDRwAEBVl4AoKCgKVAwIDUADighAAAaAAAA/wBNWTNORDkxVjFTQlQKAAAA/ABERUxMIFAyNDE4RAogAAAA/QAxVh1xHAAKICAgICAgARsCAxuxUJAFBAMCBxYBBhESFRMUHyBlAwwAEAACOoAYcTgtQFgsRQAOKCEAAB4BHYAYcRwWIFgsJQAOKCEAAJ6/FgCggDgTQDAgOgAOKCEAABp+OQCggDgfQDAgOgAOKCEAABoAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA2A"
}
```

<a id="property_atmos"></a>
## *atmos [<sup>property</sup>](#head_Properties)*

Provides access to the atmos support by this device.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | boolean | mandatory | True if device supports Atmos, false otherwise |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.atmos"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": false
}
```

<a id="property_cec"></a>
## *cec [<sup>property</sup>](#head_Properties)*

Provides access to the CEC support by this device.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | boolean | mandatory | True if device supports CEC, false otherwise |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.cec"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": false
}
```

<a id="property_version"></a>
## *version [<sup>property</sup>](#head_Properties)*

Provides access to the software version (in form *version#hashtag*) param value: Software version of the device (e.g. 1.0#14452f612c3747645d54974255d11b8f3b4faa54).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Software version (in form *version#hashtag*) param value: Software version of the device (e.g. 1.0#14452f612c3747645d54974255d11b8f3b4faa54) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.version"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "..."
}
```

<a id="property_uptime"></a>
## *uptime [<sup>property</sup>](#head_Properties)*

Provides access to the system uptime (in seconds) param value: System uptime in seconds (e.g. 120).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | System uptime (in seconds) param value: System uptime in seconds (e.g. 120) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.uptime"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": 0
}
```

<a id="property_totalram"></a>
## *totalram [<sup>property</sup>](#head_Properties)*

Provides access to the total installed system RAM (in bytes) param value: Total installed system RAM in bytes (e.g. 655757312).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Total installed system RAM (in bytes) param value: Total installed system RAM in bytes (e.g. 655757312) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.totalram"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": 0
}
```

<a id="property_freeram"></a>
## *freeram [<sup>property</sup>](#head_Properties)*

Provides access to the free system RAM (in bytes) param value: System RAM still available in bytes (e.g. 563015680).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Free system RAM (in bytes) param value: System RAM still available in bytes (e.g. 563015680) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.freeram"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": 0
}
```

<a id="property_totalswap"></a>
## *totalswap [<sup>property</sup>](#head_Properties)*

Provides access to the total swap space (in bytes) param value: Total swap space in bytes (e.g. 789132680).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Total swap space (in bytes) param value: Total swap space in bytes (e.g. 789132680) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.totalswap"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": 0
}
```

<a id="property_freeswap"></a>
## *freeswap [<sup>property</sup>](#head_Properties)*

Provides access to the free swap space (in bytes) param value: Swap space still available in bytes (e.g. 789132680).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Free swap space (in bytes) param value: Swap space still available in bytes (e.g. 789132680) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.freeswap"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": 0
}
```

<a id="property_devicename"></a>
## *devicename [<sup>property</sup>](#head_Properties)*

Provides access to the host name param value: Host name of the device (e.g. buildroot).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Host name param value: Host name of the device (e.g. buildroot) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.devicename"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "..."
}
```

<a id="property_cpuload"></a>
## *cpuload [<sup>property</sup>](#head_Properties)*

Provides access to the current CPU load (percentage) param value: Current CPU load in percentage (e.g. 2).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Current CPU load (percentage) param value: Current CPU load in percentage (e.g. 2) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.cpuload"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": 0
}
```

<a id="property_cpuloadavgs"></a>
## *cpuloadavgs [<sup>property</sup>](#head_Properties)*

Provides access to the CPU load averages from 1, 5 and 15 minutes (percentage) param value: An array of three CPU load averages in percentage (e.g. [2, 3, 4]).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | object | mandatory | CPU load averages from 1, 5 and 15 minutes (percentage) param value: An array of three CPU load averages in percentage (e.g. [2, 3, 4]) |
| (property).avg1min | integer | mandatory | Avarage CPU load over the last minute |
| (property).avg5min | integer | mandatory | Avarage CPU load over the last 5 minutes |
| (property).avg15min | integer | mandatory | Avarage CPU load over the last 15 minutes |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.cpuloadavgs"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "avg1min": 10,
    "avg5min": 15,
    "avg15min": 20
  }
}
```

<a id="property_addresses"></a>
## *addresses [<sup>property</sup>](#head_Properties)*

Provides access to the network interface addresses.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | An array of network interfaces with their addresses |
| (property)[#] | object | mandatory | *...* |
| (property)[#].name | string | mandatory | Name of the network interface |
| (property)[#].mac | string | mandatory | MAC address of the network interface |
| (property)[#]?.ipaddresses | array | optional | List of IP addresses assigned to the network interface<br>*Array length must be at most 100 elements.* |
| (property)[#]?.ipaddresses[#] | string | mandatory | *...* |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.addresses"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    {
      "name": "lo",
      "mac": "00:00:00:00:00:00",
      "ipaddresses": [
        "..."
      ]
    }
  ]
}
```

<a id="property_socketinfo"></a>
## *socketinfo [<sup>property</sup>](#head_Properties)*

Provides access to the socket information.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | object | mandatory | Information about the sockets |
| (property)?.total | integer | optional | Total number of sockets |
| (property)?.open | integer | optional | Number of open sockets |
| (property)?.link | integer | optional | Number of linked sockets |
| (property)?.exception | integer | optional | Number of exceptions |
| (property)?.shutdown | integer | optional | Number of sockets that were shut down |
| (property).runs | integer | mandatory | Number of resource monitor runs |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.socketinfo"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "total": 1024,
    "open": 512,
    "link": 256,
    "exception": 128,
    "shutdown": 64,
    "runs": 32
  }
}
```

