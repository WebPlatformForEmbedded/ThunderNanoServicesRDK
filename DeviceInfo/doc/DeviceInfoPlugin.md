<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.Device_Info_Plugin"></a>
# Device Info Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

DeviceInfo plugin for Thunder framework.

### Table of Contents

- [Introduction](#head.Introduction)
- [Description](#head.Description)
- [Configuration](#head.Configuration)
- [Interfaces](#head.Interfaces)
- [Methods](#head.Methods)
- [Properties](#head.Properties)

<a name="head.Introduction"></a>
# Introduction

<a name="head.Scope"></a>
## Scope

This document describes purpose and functionality of the DeviceInfo plugin. It includes detailed specification about its configuration, methods and properties provided.

<a name="head.Case_Sensitivity"></a>
## Case Sensitivity

All identifiers of the interfaces described in this document are case-sensitive. Thus, unless stated otherwise, all keywords, entities, properties, relations and actions should be treated as such.

<a name="head.Acronyms,_Abbreviations_and_Terms"></a>
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

<a name="head.References"></a>
## References

| Ref ID | Description |
| :-------- | :-------- |
| <a name="ref.HTTP">[HTTP](http://www.w3.org/Protocols)</a> | HTTP specification |
| <a name="ref.JSON-RPC">[JSON-RPC](https://www.jsonrpc.org/specification)</a> | JSON-RPC 2.0 specification |
| <a name="ref.JSON">[JSON](http://www.json.org/)</a> | JSON specification |
| <a name="ref.Thunder">[Thunder](https://github.com/WebPlatformForEmbedded/Thunder/blob/master/doc/WPE%20-%20API%20-%20Thunder.docx)</a> | Thunder API Reference |

<a name="head.Description"></a>
# Description

The DeviceInfo plugin allows retrieving of various device-related information.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
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

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- [DeviceInfo.json](https://github.com/rdkcentral/ThunderInterfaces/blob/master/jsonrpc/DeviceInfo.json) (version 1.0.0) (uncompliant-extended format)

<a name="head.Methods"></a>
# Methods

The following methods are provided by the DeviceInfo plugin:

DeviceInfo interface methods:

| Method | Description |
| :-------- | :-------- |
| [supportedresolutions](#method.supportedresolutions) | Supported resolutions on the selected video display port |
| [defaultresolution](#method.defaultresolution) | Default resolution on the selected video display port |
| [supportedhdcp](#method.supportedhdcp) | Supported hdcp version on the selected video display port |
| [audiocapabilities](#method.audiocapabilities) | Audio capabilities for the specified audio port |
| [ms12capabilities](#method.ms12capabilities) | Audio ms12 capabilities for the specified audio port |
| [supportedms12audioprofiles](#method.supportedms12audioprofiles) | Supported ms12 audio profiles for the specified audio port |

<a name="method.supportedresolutions"></a>
## *supportedresolutions [<sup>method</sup>](#head.Methods)*

Supported resolutions on the selected video display port.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.videoDisplay | string | mandatory | Video output supported by the device (must be one of the following: *COMPONET, COMPOSITE, DISPLAYPORT, HDMI0, HDMI1, OTHER, RF_MODULATOR, SCART_RGB, SVIDEO*) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | *...* |
| result.supportedResolutions | array | mandatory | An array of resolution supported by the device |
| result.supportedResolutions[#] | string | mandatory | Resolution supported by the device (must be one of the following: *1080i, 1080i25, 1080i50, 1080i60, 1080p, 1080p24, 1080p25, 1080p30, 1080p50, 1080p60, 2160p24, 2160p25, 2160p30, 2160p50, 2160p60, 4320p30, 4320p60, 480i, 480p, 576i, 576p, 576p50, 720p, 720p24, 720p25, 720p30, 720p50, 720p60, unknown*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | general error |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.supportedresolutions",
  "params": {
    "videoDisplay": "displayport"
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "supportedResolutions": [
      "1080p"
    ]
  }
}
```

<a name="method.defaultresolution"></a>
## *defaultresolution [<sup>method</sup>](#head.Methods)*

Default resolution on the selected video display port.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.videoDisplay | string | mandatory | Video output supported by the device (must be one of the following: *COMPONET, COMPOSITE, DISPLAYPORT, HDMI0, HDMI1, OTHER, RF_MODULATOR, SCART_RGB, SVIDEO*) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | *...* |
| result.defaultResolution | string | mandatory | Resolution supported by the device (must be one of the following: *1080i, 1080i25, 1080i50, 1080i60, 1080p, 1080p24, 1080p25, 1080p30, 1080p50, 1080p60, 2160p24, 2160p25, 2160p30, 2160p50, 2160p60, 4320p30, 4320p60, 480i, 480p, 576i, 576p, 576p50, 720p, 720p24, 720p25, 720p30, 720p50, 720p60, unknown*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | general error |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.defaultresolution",
  "params": {
    "videoDisplay": "displayport"
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "defaultResolution": "1080p"
  }
}
```

<a name="method.supportedhdcp"></a>
## *supportedhdcp [<sup>method</sup>](#head.Methods)*

Supported hdcp version on the selected video display port.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.videoDisplay | string | mandatory | Video output supported by the device (must be one of the following: *COMPONET, COMPOSITE, DISPLAYPORT, HDMI0, HDMI1, OTHER, RF_MODULATOR, SCART_RGB, SVIDEO*) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | *...* |
| result.supportedHDCPVersion | string | mandatory | HDCP support (must be one of the following: *1.4, 2.0, 2.1, 2.2, unavailable*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | general error |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.supportedhdcp",
  "params": {
    "videoDisplay": "displayport"
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "supportedHDCPVersion": "hdcp_20"
  }
}
```

<a name="method.audiocapabilities"></a>
## *audiocapabilities [<sup>method</sup>](#head.Methods)*

Audio capabilities for the specified audio port.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.audioPort | string | mandatory | Audio output supported by the device (must be one of the following: *ANALOG, DISPLAYPORT, HDMI0, HDMI1, OTHER, RF_MODULATOR, SPDIF0*) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | *...* |
| result.AudioCapabilities | array | mandatory | An array of audio capabilities |
| result.AudioCapabilities[#] | string | mandatory | Audio capability (must be one of the following: *ATMOS, DAPv2, DOLBY DIGITAL, DOLBY DIGITAL PLUS, Dual Audio Decode, MS12, none*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | general error |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.audiocapabilities",
  "params": {
    "audioPort": "analog"
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "AudioCapabilities": [
      "none"
    ]
  }
}
```

<a name="method.ms12capabilities"></a>
## *ms12capabilities [<sup>method</sup>](#head.Methods)*

Audio ms12 capabilities for the specified audio port.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.audioPort | string | mandatory | Audio output supported by the device (must be one of the following: *ANALOG, DISPLAYPORT, HDMI0, HDMI1, OTHER, RF_MODULATOR, SPDIF0*) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | *...* |
| result.MS12Capabilities | array | mandatory | An array of MS12 audio capabilities |
| result.MS12Capabilities[#] | string | mandatory | MS12 audio capability (must be one of the following: *Dialogue Enhancer, Dolby Volume, Inteligent Equalizer, none*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | general error |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.ms12capabilities",
  "params": {
    "audioPort": "analog"
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "MS12Capabilities": [
      "dolby_volume"
    ]
  }
}
```

<a name="method.supportedms12audioprofiles"></a>
## *supportedms12audioprofiles [<sup>method</sup>](#head.Methods)*

Supported ms12 audio profiles for the specified audio port.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.audioPort | string | mandatory | Audio output supported by the device (must be one of the following: *ANALOG, DISPLAYPORT, HDMI0, HDMI1, OTHER, RF_MODULATOR, SPDIF0*) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | *...* |
| result.supportedMS12AudioProfiles | array | mandatory | An array of ms12 audio profiles |
| result.supportedMS12AudioProfiles[#] | string | mandatory | MS12 Profile (must be one of the following: *Movie, Music, None, Voice*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | General error |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.supportedms12audioprofiles",
  "params": {
    "audioPort": "analog"
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "supportedMS12AudioProfiles": [
      "music"
    ]
  }
}
```

<a name="head.Properties"></a>
# Properties

The following properties are provided by the DeviceInfo plugin:

DeviceInfo interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [deviceaudiocapabilities](#property.deviceaudiocapabilities) | read-only | Audio capabilities of the device |
| [devicevideocapabilities](#property.devicevideocapabilities) | read-only | Video capabilities of the device |
| [deviceinfo](#property.deviceinfo) | read-only | Device meta data |
| [systeminfo](#property.systeminfo) | read-only | System general information |
| [addresses](#property.addresses) | read-only | Network interface addresses |
| [socketinfo](#property.socketinfo) | read-only | Socket information |
| [supportedaudioports](#property.supportedaudioports) | read-only | Audio ports supported on the device (all ports that are physically present) |
| [supportedvideodisplays](#property.supportedvideodisplays) | read-only | Video ports supported on the device (all ports that are physically present) |
| [hostedid](#property.hostedid) | read-only | EDID of the host |
| [firmwareversion](#property.firmwareversion) | read-only | Versions maintained in version |
| [serialnumber](#property.serialnumber) | read-only | Serial number set by manufacturer |
| [make](#property.make) | read-only | Device manufacturer |
| [modelid](#property.modelid) | read-only | Device model number or SKU |
| [modelname](#property.modelname) | read-only | Device model name |
| [modelyear](#property.modelyear) | read-only | Device model year |
| [friendlyname](#property.friendlyname) | read-only | Device friendly name |
| [platformname](#property.platformname) | read-only | Device Platform name |
| [devicetype](#property.devicetype) | read-only | Device type |
| [distributorid](#property.distributorid) | read-only | Partner ID or distributor ID for device |

<a name="property.deviceaudiocapabilities"></a>
## *deviceaudiocapabilities [<sup>property</sup>](#head.Properties)*

Provides access to the audio capabilities of the device.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Audio capabilities of the device |
| result.audiooutputcapabilities | array | mandatory | An array of audiooutputcapabilities |
| result.audiooutputcapabilities[#] | object | mandatory | Audio capabilities of the output |
| result.audiooutputcapabilities[#]?.audioPort | string | optional | Audio Output support (must be one of the following: *ANALOG, DISPLAYPORT, HDMI0, HDMI1, OTHER, RF_MODULATOR, SPDIF0*) |
| result.audiooutputcapabilities[#].audiocapabilities | array | mandatory | Audio capabilities for the specified audio port |
| result.audiooutputcapabilities[#].audiocapabilities[#] | string | mandatory | Audio capability (must be one of the following: *ATMOS, DAPv2, DOLBY DIGITAL, DOLBY DIGITAL PLUS, Dual Audio Decode, MS12, none*) |
| result.audiooutputcapabilities[#].ms12capabilities | array | mandatory | Audio ms12 capabilities for the specified audio port |
| result.audiooutputcapabilities[#].ms12capabilities[#] | string | mandatory | MS12 audio capability (must be one of the following: *Dialogue Enhancer, Dolby Volume, Inteligent Equalizer, none*) |
| result.audiooutputcapabilities[#].ms12profiles | array | mandatory | Audio ms12 profiles for the specified audio port |
| result.audiooutputcapabilities[#].ms12profiles[#] | string | mandatory | MS12 Profile (must be one of the following: *Movie, Music, None, Voice*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.deviceaudiocapabilities"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "audiooutputcapabilities": [
      {
        "audioPort": "analog",
        "audiocapabilities": [
          "none"
        ],
        "ms12capabilities": [
          "dolby_volume"
        ],
        "ms12profiles": [
          "music"
        ]
      }
    ]
  }
}
```

<a name="property.devicevideocapabilities"></a>
## *devicevideocapabilities [<sup>property</sup>](#head.Properties)*

Provides access to the video capabilities of the device.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Video capabilities of the device |
| result.hostedid | string | mandatory | EDID of the host |
| result.hdr | boolean | mandatory | Is HDR supported by this device |
| result.atmos | boolean | mandatory | Is Atmos supported by this device |
| result.cec | boolean | mandatory | Is CEC supported by this device |
| result.videooutputcapabilities | array | mandatory | An array of videooutputcapabilities |
| result.videooutputcapabilities[#] | object | mandatory | Video capabilities of the output |
| result.videooutputcapabilities[#].hdcp | string | mandatory | HDCP support (must be one of the following: *1.4, 2.0, 2.1, 2.2, unavailable*) |
| result.videooutputcapabilities[#]?.videoDisplay | string | optional | Video Output support (must be one of the following: *COMPONET, COMPOSITE, DISPLAYPORT, HDMI0, HDMI1, OTHER, RF_MODULATOR, SCART_RGB, SVIDEO*) |
| result.videooutputcapabilities[#].output_resolutions | array | mandatory | Supported resolutions |
| result.videooutputcapabilities[#].output_resolutions[#] | string | mandatory | Resolution supported by the device (must be one of the following: *1080i, 1080i25, 1080i50, 1080i60, 1080p, 1080p24, 1080p25, 1080p30, 1080p50, 1080p60, 2160p24, 2160p25, 2160p30, 2160p50, 2160p60, 4320p30, 4320p60, 480i, 480p, 576i, 576p, 576p50, 720p, 720p24, 720p25, 720p30, 720p50, 720p60, unknown*) |
| result.videooutputcapabilities[#].defaultresolution | string | mandatory | Default resolution (must be one of the following: *1080i, 1080i25, 1080i50, 1080i60, 1080p, 1080p24, 1080p25, 1080p30, 1080p50, 1080p60, 2160p24, 2160p25, 2160p30, 2160p50, 2160p60, 4320p30, 4320p60, 480i, 480p, 576i, 576p, 576p50, 720p, 720p24, 720p25, 720p30, 720p50, 720p60, unknown*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.devicevideocapabilities"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "hostedid": "...",
    "hdr": false,
    "atmos": false,
    "cec": true,
    "videooutputcapabilities": [
      {
        "hdcp": "hdcp_20",
        "videoDisplay": "displayport",
        "output_resolutions": [
          "1080p"
        ],
        "defaultresolution": "1080p"
      }
    ]
  }
}
```

<a name="property.deviceinfo"></a>
## *deviceinfo [<sup>property</sup>](#head.Properties)*

Provides access to the device meta data.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Device meta data |
| result?.devicetype | string | optional | Device type |
| result?.friendlyname | string | optional | Friendly name |
| result?.distributorid | string | optional | Partner ID or distributor ID for device |
| result?.make | string | optional | Device manufacturer |
| result?.modelname | string | optional | Model Name |
| result?.modelyear | integer | optional | Model Year |
| result?.platformname | string | optional | Platform name |
| result?.serialnumber | string | optional | Device serial number |
| result?.sku | string | optional | Device model number or SKU |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.deviceinfo"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "devicetype": "IpStb",
    "friendlyname": "my device",
    "distributorid": "Comcast",
    "make": "pace",
    "modelname": "model A",
    "modelyear": 2020,
    "platformname": "linux",
    "serialnumber": "WPEuCfrLF45",
    "sku": "PX051AEI"
  }
}
```

<a name="property.systeminfo"></a>
## *systeminfo [<sup>property</sup>](#head.Properties)*

Provides access to the system general information.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | System general information |
| result.version | string | mandatory | Software version (in form *version#hashtag*) |
| result.uptime | integer | mandatory | System uptime (in seconds) |
| result.totalram | integer | mandatory | Total installed system RAM memory (in bytes) |
| result.freeram | integer | mandatory | Free system RAM memory (in bytes) |
| result.totalswap | integer | mandatory | Total swap space (in bytes) |
| result.freeswap | integer | mandatory | Swap space still available (in bytes) |
| result.devicename | string | mandatory | Host name |
| result.cpuload | string | mandatory | Current CPU load (percentage) |
| result.cpuloadavg | object | mandatory | CPU load average |
| result.cpuloadavg.avg1min | integer | mandatory | 1min cpuload average |
| result.cpuloadavg.avg5min | integer | mandatory | 5min cpuload average |
| result.cpuloadavg.avg15min | integer | mandatory | 15min cpuload average |
| result.serialnumber | string | mandatory | Device serial number |
| result.time | string | mandatory | Current system date and time |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DeviceInfo.1.systeminfo"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "version": "1.0#14452f612c3747645d54974255d11b8f3b4faa54",
    "uptime": 120,
    "totalram": 655757312,
    "freeram": 563015680,
    "totalswap": 789132680,
    "freeswap": 789132680,
    "devicename": "buildroot",
    "cpuload": "2",
    "cpuloadavg": {
      "avg1min": 789132680,
      "avg5min": 789132680,
      "avg15min": 789132680
    },
    "serialnumber": "WPEuCfrLF45",
    "time": "Mon, 11 Mar 2019 14:38:18"
  }
}
```

<a name="property.addresses"></a>
## *addresses [<sup>property</sup>](#head.Properties)*

Provides access to the network interface addresses.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | Network interface addresses |
| result[#] | object | mandatory | *...* |
| result[#].name | string | mandatory | Interface name |
| result[#].mac | string | mandatory | Interface MAC address |
| result[#]?.ip | array | optional | An array of Interface IP address |
| result[#]?.ip[#] | string | optional | Interface IP address |

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
      "mac": "00:00:00:00:00",
      "ip": [
        "127.0.0.1"
      ]
    }
  ]
}
```

<a name="property.socketinfo"></a>
## *socketinfo [<sup>property</sup>](#head.Properties)*

Provides access to the socket information.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Socket information |
| result?.total | integer | optional | Number of total |
| result?.open | integer | optional | Number of open |
| result?.link | integer | optional | Number of link |
| result?.exception | integer | optional | Number of exception |
| result?.shutdown | integer | optional | Number of shutdown |
| result.runs | integer | mandatory | Number of runs |

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
    "total": 0,
    "open": 0,
    "link": 0,
    "exception": 0,
    "shutdown": 0,
    "runs": 1
  }
}
```

<a name="property.supportedaudioports"></a>
## *supportedaudioports [<sup>property</sup>](#head.Properties)*

Provides access to the audio ports supported on the device (all ports that are physically present).

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Audio ports supported on the device (all ports that are physically present) |
| result.supportedAudioPorts | array | mandatory | Audio Output support |
| result.supportedAudioPorts[#] | string | mandatory | Audio output supported by the device (must be one of the following: *ANALOG, DISPLAYPORT, HDMI0, HDMI1, OTHER, RF_MODULATOR, SPDIF0*) |

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
  "method": "DeviceInfo.1.supportedaudioports"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "supportedAudioPorts": [
      "analog"
    ]
  }
}
```

<a name="property.supportedvideodisplays"></a>
## *supportedvideodisplays [<sup>property</sup>](#head.Properties)*

Provides access to the video ports supported on the device (all ports that are physically present).

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Video ports supported on the device (all ports that are physically present) |
| result.supportedVideoDisplays | array | mandatory | Video Output support |
| result.supportedVideoDisplays[#] | string | mandatory | Video output supported by the device (must be one of the following: *COMPONET, COMPOSITE, DISPLAYPORT, HDMI0, HDMI1, OTHER, RF_MODULATOR, SCART_RGB, SVIDEO*) |

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
  "method": "DeviceInfo.1.supportedvideodisplays"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "supportedVideoDisplays": [
      "displayport"
    ]
  }
}
```

<a name="property.hostedid"></a>
## *hostedid [<sup>property</sup>](#head.Properties)*

Provides access to the EDID of the host.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | EDID of the host |
| result.EDID | string | mandatory | A base64 encoded byte array string representing the EDID |

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
  "method": "DeviceInfo.1.hostedid"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "EDID": "AP///////wAQrMLQVEJTMQUdAQOANR546q11qVRNnSYPUFSlSwCBALMA0QBxT6lAgYDRwAEBVl4AoKCgKVAwIDUADighAAAaAAAA/wBNWTNORDkxVjFTQlQKAAAA/ABERUxMIFAyNDE4RAogAAAA/QAxVh1xHAAKICAgICAgARsCAxuxUJAFBAMCBxYBBhESFRMUHyBlAwwAEAACOoAYcTgtQFgsRQAOKCEAAB4BHYAYcRwWIFgsJQAOKCEAAJ6/FgCggDgTQDAgOgAOKCEAABp+OQCggDgfQDAgOgAOKCEAABoAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA2A"
  }
}
```

<a name="property.firmwareversion"></a>
## *firmwareversion [<sup>property</sup>](#head.Properties)*

Provides access to the versions maintained in version.txt.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Versions maintained in version.txt |
| result.imagename | string | mandatory | Name of firmware image |
| result?.sdk | string | optional | SDK version string |
| result?.mediarite | string | optional | Mediarite value |
| result?.yocto | string | optional | Yocto version (must be one of the following: *daisy, dunfell, kirkstone, morty*) |

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
  "method": "DeviceInfo.1.firmwareversion"
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
    "yocto": "dunfell"
  }
}
```

<a name="property.serialnumber"></a>
## *serialnumber [<sup>property</sup>](#head.Properties)*

Provides access to the serial number set by manufacturer.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Serial number set by manufacturer |
| result.serialnumber | string | mandatory | Device Serial Number |

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
  "method": "DeviceInfo.1.serialnumber"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "serialnumber": "PAW400003744"
  }
}
```

<a name="property.make"></a>
## *make [<sup>property</sup>](#head.Properties)*

Provides access to the device manufacturer.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Device manufacturer |
| result.make | string | mandatory | Device manufacturer (must be one of the following: *Amlogic_Inc, Pioneer, arris, cisco, commscope, element, hisense, llama, pace, platco, raspberrypi_org, samsung, sercomm, sky, technicolor*) |

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
  "method": "DeviceInfo.1.make"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "make": "pace"
  }
}
```

<a name="property.modelid"></a>
## *modelid [<sup>property</sup>](#head.Properties)*

Provides access to the device model number or SKU.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Device model number or SKU |
| result.sku | string | mandatory | Device model number or SKU (must be one of the following: *AX013AN, AX014AN, AX061AEI, CMXI11BEI, CS011AN, CXD01ANI, ELTE11MWR, HSTP11MWR, HSTP11MWRFX50, LS301, MX011AN, PI, PITU11MWR, PLTL11AEI, PX001AN, PX013AN, PX022AN, PX032ANI, PX051AEI, PXD01ANI, SCXI11AIC, SCXI11BEI, SKTL11AEI, SKXI11ADS, SKXI11AIS, SKXI11ANS, SX022AN, TX061AEI, XUSHTB11MWR, XUSHTC11MWR, XUSPTC11MWR, ZWCN11MWI*) |

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
  "method": "DeviceInfo.1.modelid"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "sku": "PX051AEI"
  }
}
```

<a name="property.modelname"></a>
## *modelname [<sup>property</sup>](#head.Properties)*

Provides access to the device model name.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Device model name |
| result.model | string | mandatory | Device model name |

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
  "method": "DeviceInfo.1.modelname"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "model": "Pace Xi5"
  }
}
```

<a name="property.modelyear"></a>
## *modelyear [<sup>property</sup>](#head.Properties)*

Provides access to the device model year.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Device model year |
| result.year | integer | mandatory | Device model year |

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
  "method": "DeviceInfo.1.modelyear"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "year": 2020
  }
}
```

<a name="property.friendlyname"></a>
## *friendlyname [<sup>property</sup>](#head.Properties)*

Provides access to the device friendly name.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Device friendly name |
| result.name | string | mandatory | Device friendly name |

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
  "method": "DeviceInfo.1.friendlyname"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "name": "My device"
  }
}
```

<a name="property.platformname"></a>
## *platformname [<sup>property</sup>](#head.Properties)*

Provides access to the device Platform name.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Device Platform name |
| result.name | string | mandatory | Device Platform name |

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
  "method": "DeviceInfo.1.platformname"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "name": "Linux"
  }
}
```

<a name="property.devicetype"></a>
## *devicetype [<sup>property</sup>](#head.Properties)*

Provides access to the device type.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Device type |
| result.devicetype | string | mandatory | Device type (must be one of the following: *IpStb, QamIpStb, hybrid, mediaclient, tv*) |

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
  "method": "DeviceInfo.1.devicetype"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "devicetype": "IpStb"
  }
}
```

<a name="property.distributorid"></a>
## *distributorid [<sup>property</sup>](#head.Properties)*

Provides access to the partner ID or distributor ID for device.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Partner ID or distributor ID for device |
| result.distributorid | string | mandatory | Partner ID or distributor ID for device (must be one of the following: *MIT, charter, charter-dev, comcast, cox, cox-dev, cox-hospitality, cox-qa, rogers, rogers-dev, shaw, shaw-dev, sky-de, sky-deu, sky-deu-dev, sky-it, sky-it-dev, sky-italia, sky-uk, sky-uk-dev, videotron, xglobal*) |

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
  "method": "DeviceInfo.1.distributorid"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "distributorid": "comcast"
  }
}
```

