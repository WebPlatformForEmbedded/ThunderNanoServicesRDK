<!-- Generated automatically, DO NOT EDIT! -->
<a id="head_Player_Info_Plugin"></a>
# Player Info Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::white_circle:**

PlayerInfo plugin for Thunder framework.

### Table of Contents

- [Introduction](#head_Introduction)
- [Description](#head_Description)
- [Configuration](#head_Configuration)
- [Interfaces](#head_Interfaces)
- [Methods](#head_Methods)
- [Properties](#head_Properties)
- [Notifications](#head_Notifications)

<a id="head_Introduction"></a>
# Introduction

<a id="head_Scope"></a>
## Scope

This document describes purpose and functionality of the PlayerInfo plugin. It includes detailed specification about its configuration, methods and properties as well as sent notifications.

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

The PlayerInfo plugin helps to get system supported Audio Video codecs.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a id="head_Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *PlayerInfo*) |
| classname | string | mandatory | Class name: *PlayerInfo* |
| locator | string | mandatory | Library name: *libWPEPlayerInfo.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |

<a id="head_Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- IPlayerProperties ([IPlayerInfo.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IPlayerInfo.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- Dolby::IOutput ([IDolby.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IDolby.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a id="head_Methods"></a>
# Methods

The following methods are provided by the PlayerInfo plugin:

Built-in methods:

| Method | Description |
| :-------- | :-------- |
| [versions](#method_versions) | Retrieves a list of JSON-RPC interfaces offered by this service |
| [exists](#method_exists) | Checks if a JSON-RPC method or property exists |
| [register](#method_register) | Registers for an asynchronous JSON-RPC notification |
| [unregister](#method_unregister) | Unregisters from an asynchronous JSON-RPC notification |

<a id="method_versions"></a>
## *versions [<sup>method</sup>](#head_Methods)*

Retrieves a list of JSON-RPC interfaces offered by this service.

### Parameters

This method takes no parameters.

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | A list ofsinterfaces with their version numbers<br>*Array length must be at most 255 elements.* |
| result[#] | object | mandatory | *...* |
| result[#].name | string | mandatory | Name of the interface |
| result[#].major | integer | mandatory | Major part of version number |
| result[#].minor | integer | mandatory | Minor part of version number |
| result[#].patch | integer | mandatory | Patch part of version version number |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.versions"
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    {
      "name": "JController",
      "major": 1,
      "minor": 0,
      "patch": 0
    }
  ]
}
```

<a id="method_exists"></a>
## *exists [<sup>method</sup>](#head_Methods)*

Checks if a JSON-RPC method or property exists.

### Description

This method will return *True* for the following methods/properties: *audiocodecs, videocodecs, resolution, isaudioequivalenceenabled, dolbyatmossupported, dolbysoundmode, dolbyatmosoutput, dolbymode, versions, exists, register, unregister*.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.method | string | mandatory | Name of the method or property to look up |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | boolean | mandatory | Denotes if the method exists or not |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.exists",
  "params": {
    "method": "audiocodecs"
  }
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

<a id="method_register"></a>
## *register [<sup>method</sup>](#head_Methods)*

Registers for an asynchronous JSON-RPC notification.

### Description

This method supports the following event names: *[soundmodechanged](#notification_soundmodechanged)*.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.event | string | mandatory | Name of the notification to register for |
| params.id | string | mandatory | Client identifier |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | null | mandatory | Always null |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_FAILED_REGISTERED``` | Failed to register for the notification (e.g. already registered) |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.register",
  "params": {
    "event": "soundmodechanged",
    "id": "myapp"
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": null
}
```

<a id="method_unregister"></a>
## *unregister [<sup>method</sup>](#head_Methods)*

Unregisters from an asynchronous JSON-RPC notification.

### Description

This method supports the following event names: *[soundmodechanged](#notification_soundmodechanged)*.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.event | string | mandatory | Name of the notification to register for |
| params.id | string | mandatory | Client identifier |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | null | mandatory | Always null |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_FAILED_UNREGISTERED``` | Failed to unregister from the notification (e.g. not yet registered) |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.unregister",
  "params": {
    "event": "soundmodechanged",
    "id": "myapp"
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": null
}
```

<a id="head_Properties"></a>
# Properties

The following properties are provided by the PlayerInfo plugin:

PlayerProperties interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [audiocodecs](#property_audiocodecs) | read-only | Query Audio Codecs List |
| [videocodecs](#property_videocodecs) | read-only | Query Video Codecs List |
| [resolution](#property_resolution) | read-only | Current Video playback resolution |
| [isaudioequivalenceenabled](#property_isaudioequivalenceenabled) | read-only | Checks Loudness Equivalence in platform |

Dolby Output interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [dolbyatmossupported](#property_dolbyatmossupported) / [dolby_atmosmetadata](#property_dolbyatmossupported) | read-only | Atmos capabilities of Sink |
| [dolbysoundmode](#property_dolbysoundmode) / [dolby_soundmode](#property_dolbysoundmode) | read-only | Sound Mode - Mono/Stereo/Surround |
| [dolbyatmosoutput](#property_dolbyatmosoutput) / [dolby_enableatmosoutput](#property_dolbyatmosoutput) | write-only | Enable Atmos Audio Output |
| [dolbymode](#property_dolbymode) / [dolby_mode](#property_dolbymode) | read/write | Dolby Mode |

<a id="property_audiocodecs"></a>
## *audiocodecs [<sup>property</sup>](#head_Properties)*

Provides access to the query Audio Codecs List.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | Query Audio Codecs List |
| (property)[#] | string | mandatory | *...* (must be one of the following: *AudioAac, AudioAc3, AudioAc3Plus, AudioDts, AudioMpeg1, AudioMpeg2, AudioMpeg3, AudioMpeg4, AudioOpus, AudioUndefined, AudioVorbisOgg, AudioWav*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.audiocodecs"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "AudioAac"
  ]
}
```

<a id="property_videocodecs"></a>
## *videocodecs [<sup>property</sup>](#head_Properties)*

Provides access to the query Video Codecs List.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | Query Video Codecs List |
| (property)[#] | string | mandatory | *...* (must be one of the following: *VideoH263, VideoH264, VideoH265, VideoH26510, VideoMpeg, VideoMpeg2, VideoMpeg4, VideoUndefined, VideoVp10, VideoVp8, VideoVp9*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.videocodecs"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "VideoH263"
  ]
}
```

<a id="property_resolution"></a>
## *resolution [<sup>property</sup>](#head_Properties)*

Provides access to the current Video playback resolution.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Current Video playback resolution (must be one of the following: *Resolution1080i, Resolution1080i24, Resolution1080i25, Resolution1080i30, Resolution1080i50, Resolution1080p, Resolution1080p24, Resolution1080p25, Resolution1080p30, Resolution1080p50, Resolution2160p, Resolution2160p24, Resolution2160p25, Resolution2160p30, Resolution2160p50, Resolution2160p60, Resolution480i, Resolution480i24, Resolution480i25, Resolution480i30, Resolution480i50, Resolution480p, Resolution480p24, Resolution480p25, Resolution480p30, Resolution480p50, Resolution576i, Resolution576i24, Resolution576i25, Resolution576i30, Resolution576i50, Resolution576p, Resolution576p24, Resolution576p25, Resolution576p30, Resolution576p50, Resolution720p, Resolution720p24, Resolution720p25, Resolution720p30, Resolution720p50, ResolutionUnknown*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.resolution"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "Resolution480i24"
}
```

<a id="property_isaudioequivalenceenabled"></a>
## *isaudioequivalenceenabled [<sup>property</sup>](#head_Properties)*

Provides access to the checks Loudness Equivalence in platform.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | boolean | mandatory | Checks Loudness Equivalence in platform |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.isaudioequivalenceenabled"
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

<a id="property_dolbyatmossupported"></a>
## *dolbyatmossupported [<sup>property</sup>](#head_Properties)*

Provides access to the atmos capabilities of Sink.

> This property is **read-only**.

> ``dolby_atmosmetadata`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | boolean | mandatory | Atmos capabilities of Sink |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.dolbyatmossupported"
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

<a id="property_dolbysoundmode"></a>
## *dolbysoundmode [<sup>property</sup>](#head_Properties)*

Provides access to the sound Mode - Mono/Stereo/Surround.

> This property is **read-only**.

> ``dolby_soundmode`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Sound Mode - Mono/Stereo/Surround (must be one of the following: *Dolbydigital, Dolbydigitalplus, Mono, Passthru, SoundmodeAuto, Stereo, Surround, Unknown*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.dolbysoundmode"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "Mono"
}
```

<a id="property_dolbyatmosoutput"></a>
## *dolbyatmosoutput [<sup>property</sup>](#head_Properties)*

Provides access to the enable Atmos Audio Output.

> This property is **write-only**.

> ``dolby_enableatmosoutput`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | boolean | mandatory | Enable/Disable |

### Example

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.dolbyatmosoutput",
  "params": false
}
```

#### Set Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": "null"
}
```

<a id="property_dolbymode"></a>
## *dolbymode [<sup>property</sup>](#head_Properties)*

Provides access to the dolby Mode.

> ``dolby_mode`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Dolby mode type (must be one of the following: *Auto, DigitalAc3, DigitalPassthrough, DigitalPcm, DigitalPlus, Ms12*) |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Dolby mode type (must be one of the following: *Auto, DigitalAc3, DigitalPassthrough, DigitalPcm, DigitalPlus, Ms12*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.dolbymode"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "DigitalPlus"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.dolbymode",
  "params": "DigitalPlus"
}
```

#### Set Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": "null"
}
```

<a id="head_Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the PlayerInfo plugin:

Dolby Output interface events:

| Notification | Description |
| :-------- | :-------- |
| [soundmodechanged](#notification_soundmodechanged) / [dolby_audiomodechanged](#notification_soundmodechanged) | Signal audio mode change |

<a id="notification_soundmodechanged"></a>
## *soundmodechanged [<sup>notification</sup>](#head_Notifications)*

Signal audio mode change.

> ``dolby_audiomodechanged`` is an alternative name for this notification. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.mode | string | mandatory | Changed Mode (must be one of the following: *Dolbydigital, Dolbydigitalplus, Mono, Passthru, SoundmodeAuto, Stereo, Surround, Unknown*) |
| params.enabled | boolean | mandatory | Enabled/Disabled |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.register",
  "params": {
    "event": "soundmodechanged",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.soundmodechanged",
  "params": {
    "mode": "Mono",
    "enabled": false
  }
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.soundmodechanged``.

