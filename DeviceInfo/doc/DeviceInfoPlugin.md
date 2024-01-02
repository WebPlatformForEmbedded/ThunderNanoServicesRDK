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
| <a name="ref.Thunder">[Thunder](https://github.com/WebPlatformForEmbedded/Thunder/blob/master/doc/WPE%20-%20API%20-%20WPEFramework.docx)</a> | Thunder API Reference |

<a name="head.Description"></a>
# Description

The DeviceInfo plugin allows retrieving of various device-related information.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| callsign | string | Plugin instance name (default: *DeviceInfo*) |
| classname | string | Class name: *DeviceInfo* |
| locator | string | Library name: *libWPEFrameworkDeviceInfo.so* |
| startmode | string | Determines if the plugin shall be started automatically along with the framework |
| hdr | boolean | Does the device support HDR (true or false) |
| atmos | boolean | Does the device support Dolby Atmos (true or false) |
| cec | boolean | Does the device support HDMI CEC (true or false) |
| hdcp | string | HDCP version supported by the device (UNAVAILABLE, 1.4, 2.0, 2.1, 2.2) |
| audio | array | Supported audio outputs |
| audio[#] | string | Audio output (OTHER, RF_MODULATOR, ANALOG, SPDIF0, HDMI0, HDMI1, DISPLAYPORT) |
| video | array | Supported video outputs |
| video[#] | string | Video output (OTHER, RF_MODULATOR, COMPOSITE, SVIDEO, COMPONENT, SCART_RGB, HDMI0, HDMI1, DISPLAYPORT) |
| resolution | array | Supported output resolutions |
| resolution[#] | string | Output resolution (unknown, 480i, 480p, 576i, 576p, 720p, 1080i, 1080p, 2160p30, 2160p60, 4320p30, 4320p60) |
| modelName | string | Model Name |
| modelYear | number | Model Year |
| friendlyName | string | friendly Name |
| systemIntegratorName | string | system Integrator Name |
| platformName | string | platform Name |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.videoDisplay | string | Video Output support (must be one of the following: *OTHER*, *RF_MODULATOR*, *COMPOSITE*, *SVIDEO*, *COMPONET*, *SCART_RGB*, *HDMI0*, *HDMI1*, *DISPLAYPORT*) |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.supportedResolutions | array | Supported resolutions |
| result.supportedResolutions[#] | string | Default resolution (must be one of the following: *unknown*, *480i*, *480p*, *576i*, *576p*, *576p50*, *720p*, *720p50*, *1080i*, *1080i25*, *1080i50*, *1080p*, *1080p24*, *1080p25*, *1080p30*, *1080p50*, *1080p60*, *2160p30*, *2160p50*, *2160p60*, *4320p30*, *4320p60*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "DeviceInfo.1.supportedresolutions",
    "params": {
        "videoDisplay": "DISPLAYPORT"
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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.videoDisplay | string | Video Output support (must be one of the following: *OTHER*, *RF_MODULATOR*, *COMPOSITE*, *SVIDEO*, *COMPONET*, *SCART_RGB*, *HDMI0*, *HDMI1*, *DISPLAYPORT*) |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.defaultResolution | string | Default resolution (must be one of the following: *unknown*, *480i*, *480p*, *576i*, *576p*, *576p50*, *720p*, *720p50*, *1080i*, *1080i25*, *1080i50*, *1080p*, *1080p24*, *1080p25*, *1080p30*, *1080p50*, *1080p60*, *2160p30*, *2160p50*, *2160p60*, *4320p30*, *4320p60*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "DeviceInfo.1.defaultresolution",
    "params": {
        "videoDisplay": "DISPLAYPORT"
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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.videoDisplay | string | Video Output support (must be one of the following: *OTHER*, *RF_MODULATOR*, *COMPOSITE*, *SVIDEO*, *COMPONET*, *SCART_RGB*, *HDMI0*, *HDMI1*, *DISPLAYPORT*) |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.supportedHDCPVersion | string | HDCP support (must be one of the following: *unavailable*, *1.4*, *2.0*, *2.1*, *2.2*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "DeviceInfo.1.supportedhdcp",
    "params": {
        "videoDisplay": "DISPLAYPORT"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "supportedHDCPVersion": "2.0"
    }
}
```

<a name="method.audiocapabilities"></a>
## *audiocapabilities [<sup>method</sup>](#head.Methods)*

Audio capabilities for the specified audio port.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.audioPort | string | Audio Output support (must be one of the following: *OTHER*, *RF_MODULATOR*, *ANALOG*, *SPDIF0*, *HDMI0*, *HDMI1*, *DISPLAYPORT*) |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.AudioCapabilities | array | Audio capabilities for the specified audio port |
| result.AudioCapabilities[#] | string | Audio capability (must be one of the following: *none*, *ATMOS*, *DOLBY DIGITAL*, *DOLBY DIGITAL PLUS*, *Dual Audio Decode*, *DAPv2*, *MS12*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "DeviceInfo.1.audiocapabilities",
    "params": {
        "audioPort": "ANALOG"
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
            "NONE"
        ]
    }
}
```

<a name="method.ms12capabilities"></a>
## *ms12capabilities [<sup>method</sup>](#head.Methods)*

Audio ms12 capabilities for the specified audio port.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.audioPort | string | Audio Output support (must be one of the following: *OTHER*, *RF_MODULATOR*, *ANALOG*, *SPDIF0*, *HDMI0*, *HDMI1*, *DISPLAYPORT*) |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.MS12Capabilities | array | Audio ms12 capabilities for the specified audio port |
| result.MS12Capabilities[#] | string | MS12 audio capability (must be one of the following: *none*, *Dolby Volume*, *Inteligent Equalizer*, *Dialogue Enhancer*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "DeviceInfo.1.ms12capabilities",
    "params": {
        "audioPort": "ANALOG"
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
            "Dolby Volume"
        ]
    }
}
```

<a name="method.supportedms12audioprofiles"></a>
## *supportedms12audioprofiles [<sup>method</sup>](#head.Methods)*

Supported ms12 audio profiles for the specified audio port.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.audioPort | string | Audio Output support (must be one of the following: *OTHER*, *RF_MODULATOR*, *ANALOG*, *SPDIF0*, *HDMI0*, *HDMI1*, *DISPLAYPORT*) |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.supportedMS12AudioProfiles | array | An array of ms12 audio profiles |
| result.supportedMS12AudioProfiles[#] | string | MS12 Profile (must be one of the following: *None*, *Music*, *Movie*, *Voice*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "DeviceInfo.1.supportedms12audioprofiles",
    "params": {
        "audioPort": "ANALOG"
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

| Property | Description |
| :-------- | :-------- |
| [deviceaudiocapabilities](#property.deviceaudiocapabilities) <sup>RO</sup> | Audio capabilities of the device |
| [devicevideocapabilities](#property.devicevideocapabilities) <sup>RO</sup> | Video capabilities of the device |
| [deviceinfo](#property.deviceinfo) <sup>RO</sup> | Device meta data |
| [systeminfo](#property.systeminfo) <sup>RO</sup> | System general information |
| [addresses](#property.addresses) <sup>RO</sup> | Network interface addresses |
| [socketinfo](#property.socketinfo) <sup>RO</sup> | Socket information |
| [supportedaudioports](#property.supportedaudioports) <sup>RO</sup> | Audio ports supported on the device (all ports that are physically present) |
| [supportedvideodisplays](#property.supportedvideodisplays) <sup>RO</sup> | Video ports supported on the device (all ports that are physically present) |
| [hostedid](#property.hostedid) <sup>RO</sup> | EDID of the host |
| [firmwareversion](#property.firmwareversion) <sup>RO</sup> | Versions maintained in version |
| [serialnumber](#property.serialnumber) <sup>RO</sup> | Serial number set by manufacturer |
| [modelid](#property.modelid) <sup>RO</sup> | Device model number or SKU |
| [make](#property.make) <sup>RO</sup> | Device manufacturer |
| [modelname](#property.modelname) <sup>RO</sup> | Device model name |
| [modelyear](#property.modelyear) <sup>RO</sup> | Device model year |
| [friendlyname](#property.friendlyname) <sup>RO</sup> | Device friendly name |
| [platformname](#property.platformname) <sup>RO</sup> | Device Platform name |
| [devicetype](#property.devicetype) <sup>RO</sup> | Device type |
| [distributorid](#property.distributorid) <sup>RO</sup> | Partner ID or distributor ID for device |


<a name="property.deviceaudiocapabilities"></a>
## *deviceaudiocapabilities [<sup>property</sup>](#head.Properties)*

Provides access to the audio capabilities of the device.

> This property is **read-only**.

### Value

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Audio capabilities of the device |
| result.audiooutputcapabilities | array |  |
| result.audiooutputcapabilities[#] | object | Audio capabilities of the output |
| result.audiooutputcapabilities[#]?.audioPort | string | <sup>*(optional)*</sup> Audio Output support (must be one of the following: *OTHER*, *RF_MODULATOR*, *ANALOG*, *SPDIF0*, *HDMI0*, *HDMI1*, *DISPLAYPORT*) |
| result.audiooutputcapabilities[#].audiocapabilities | array | Audio capabilities for the specified audio port |
| result.audiooutputcapabilities[#].audiocapabilities[#] | string | Audio capability (must be one of the following: *none*, *ATMOS*, *DOLBY DIGITAL*, *DOLBY DIGITAL PLUS*, *Dual Audio Decode*, *DAPv2*, *MS12*) |
| result.audiooutputcapabilities[#].ms12capabilities | array | Audio ms12 capabilities for the specified audio port |
| result.audiooutputcapabilities[#].ms12capabilities[#] | string | MS12 audio capability (must be one of the following: *none*, *Dolby Volume*, *Inteligent Equalizer*, *Dialogue Enhancer*) |
| result.audiooutputcapabilities[#].ms12profiles | array | An array of ms12 audio profiles |
| result.audiooutputcapabilities[#].ms12profiles[#] | string | MS12 Profile (must be one of the following: *None*, *Music*, *Movie*, *Voice*) |

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
                "audioPort": "ANALOG",
                "audiocapabilities": [
                    "NONE"
                ],
                "ms12capabilities": [
                    "Dolby Volume"
                ],
                "ms12profiles": [
                    "Music"
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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Video capabilities of the device |
| result.hostedid | string | EDID of the host |
| result.hdr | boolean | Is HDR supported by this device |
| result.atmos | boolean | Is Atmos supported by this device |
| result.cec | boolean | Is CEC supported by this device |
| result.videooutputcapabilities | array |  |
| result.videooutputcapabilities[#] | object | Video capabilities of the output |
| result.videooutputcapabilities[#].hdcp | string | HDCP support (must be one of the following: *unavailable*, *1.4*, *2.0*, *2.1*, *2.2*) |
| result.videooutputcapabilities[#]?.videoDisplay | string | <sup>*(optional)*</sup> Video Output support (must be one of the following: *OTHER*, *RF_MODULATOR*, *COMPOSITE*, *SVIDEO*, *COMPONET*, *SCART_RGB*, *HDMI0*, *HDMI1*, *DISPLAYPORT*) |
| result.videooutputcapabilities[#].output_resolutions | array | Supported resolutions |
| result.videooutputcapabilities[#].output_resolutions[#] | string | Default resolution (must be one of the following: *unknown*, *480i*, *480p*, *576i*, *576p*, *576p50*, *720p*, *720p50*, *1080i*, *1080i25*, *1080i50*, *1080p*, *1080p24*, *1080p25*, *1080p30*, *1080p50*, *1080p60*, *2160p30*, *2160p50*, *2160p60*, *4320p30*, *4320p60*) |
| result.videooutputcapabilities[#].defaultresolution | string | Default resolution (must be one of the following: *unknown*, *480i*, *480p*, *576i*, *576p*, *576p50*, *720p*, *720p50*, *1080i*, *1080i25*, *1080i50*, *1080p*, *1080p24*, *1080p25*, *1080p30*, *1080p50*, *1080p60*, *2160p30*, *2160p50*, *2160p60*, *4320p30*, *4320p60*) |

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
                "hdcp": "2.0",
                "videoDisplay": "DISPLAYPORT",
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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Device meta data |
| result?.devicetype | string | <sup>*(optional)*</sup> Device type |
| result?.friendlyname | string | <sup>*(optional)*</sup> Friendly name |
| result?.distributorid | string | <sup>*(optional)*</sup> Partner ID or distributor ID for device |
| result?.make | string | <sup>*(optional)*</sup> Device manufacturer |
| result?.modelname | string | <sup>*(optional)*</sup> Model Name |
| result?.modelyear | number | <sup>*(optional)*</sup> Model Year |
| result?.platformname | string | <sup>*(optional)*</sup> Platform name |
| result?.serialnumber | string | <sup>*(optional)*</sup> Device serial number |
| result?.sku | string | <sup>*(optional)*</sup> Device model number or SKU |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | System general information |
| result.version | string | Software version (in form *version#hashtag*) |
| result.uptime | number | System uptime (in seconds) |
| result.totalram | number | Total installed system RAM memory (in bytes) |
| result.freeram | number | Free system RAM memory (in bytes) |
| result.totalswap | number | Total swap space (in bytes) |
| result.freeswap | number | Swap space still available (in bytes) |
| result.devicename | string | Host name |
| result.cpuload | string | Current CPU load (percentage) |
| result.cpuloadavg | object | CPU load average |
| result.cpuloadavg.avg1min | number | 1min cpuload average |
| result.cpuloadavg.avg5min | number | 5min cpuload average |
| result.cpuloadavg.avg15min | number | 15min cpuload average |
| result.serialnumber | string | Device serial number |
| result.time | string | Current system date and time |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | array | Network interface addresses |
| result[#] | object |  |
| result[#].name | string | Interface name |
| result[#].mac | string | Interface MAC address |
| result[#]?.ip | array | <sup>*(optional)*</sup>  |
| result[#]?.ip[#] | string | <sup>*(optional)*</sup> Interface IP address |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Socket information |
| result?.total | number | <sup>*(optional)*</sup>  |
| result?.open | number | <sup>*(optional)*</sup>  |
| result?.link | number | <sup>*(optional)*</sup>  |
| result?.exception | number | <sup>*(optional)*</sup>  |
| result?.shutdown | number | <sup>*(optional)*</sup>  |
| result.runs | number | Number of runs |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Audio ports supported on the device (all ports that are physically present) |
| result.supportedAudioPorts | array | Audio Output support |
| result.supportedAudioPorts[#] | string | Audio Output support (must be one of the following: *OTHER*, *RF_MODULATOR*, *ANALOG*, *SPDIF0*, *HDMI0*, *HDMI1*, *DISPLAYPORT*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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
            "ANALOG"
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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Video ports supported on the device (all ports that are physically present) |
| result.supportedVideoDisplays | array | Video Output support |
| result.supportedVideoDisplays[#] | string | Video Output support (must be one of the following: *OTHER*, *RF_MODULATOR*, *COMPOSITE*, *SVIDEO*, *COMPONET*, *SCART_RGB*, *HDMI0*, *HDMI1*, *DISPLAYPORT*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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
            "DISPLAYPORT"
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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | EDID of the host |
| result.EDID | string | A base64 encoded byte array string representing the EDID |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Versions maintained in version.txt |
| result.imagename | string |  |
| result?.sdk | string | <sup>*(optional)*</sup>  |
| result?.mediarite | string | <sup>*(optional)*</sup>  |
| result?.yocto | string | <sup>*(optional)*</sup> Yocto version (must be one of the following: *dunfell*, *morty*, *daisy*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Serial number set by manufacturer |
| result.serialnumber | string |  |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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

<a name="property.modelid"></a>
## *modelid [<sup>property</sup>](#head.Properties)*

Provides access to the device model number or SKU.

> This property is **read-only**.

### Value

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Device model number or SKU |
| result.sku | string | Device model number or SKU (must be one of the following: *PLTL11AEI*, *ZWCN11MWI*, *SKTL11AEI*, *LS301*, *HSTP11MWR*, *HSTP11MWRFX50*, *ELTE11MWR*, *SKXI11ADS*, *SKXI11AIS*, *SKXI11ANS*, *SCXI11AIC*, *SCXI11BEI*, *CMXI11BEI*, *AX013AN*, *AX014AN*, *AX061AEI*, *MX011AN*, *CS011AN*, *CXD01ANI*, *PX001AN*, *PX013AN*, *PX022AN*, *PX032ANI*, *PX051AEI*, *PXD01ANI*, *SX022AN*, *TX061AEI*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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

<a name="property.make"></a>
## *make [<sup>property</sup>](#head.Properties)*

Provides access to the device manufacturer.

> This property is **read-only**.

### Value

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Device manufacturer |
| result.make | string | Device manufacturer (must be one of the following: *platco*, *llama*, *hisense*, *element*, *sky*, *sercomm*, *commscope*, *arris*, *cisco*, *pace*, *samsung*, *technicolor*, *Amlogic_Inc*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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

<a name="property.modelname"></a>
## *modelname [<sup>property</sup>](#head.Properties)*

Provides access to the device model name.

> This property is **read-only**.

### Value

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Device model name |
| result.model | string |  |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Device model year |
| result.year | number |  |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Device friendly name |
| result.name | string |  |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Device Platform name |
| result.name | string |  |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Device type |
| result.devicetype | string | Device type (must be one of the following: *tv*, *IpStb*, *QamIpStb*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Partner ID or distributor ID for device |
| result.distributorid | string | Partner ID or distributor ID for device (must be one of the following: *comcast*, *xglobal*, *sky-de*, *sky-italia*, *sky-uk*, *sky-uk-dev*, *sky-deu*, *sky-deu-dev*, *sky-it*, *sky-it-dev*, *cox*, *cox-hospitality*, *cox-dev*, *cox-qa*, *MIT*, *shaw*, *shaw-dev*, *rogers*, *rogers-dev*, *videotron*, *charter*, *charter-dev*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```ERROR_GENERAL``` |  |

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

