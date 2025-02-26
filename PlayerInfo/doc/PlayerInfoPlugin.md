<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.Player_Info_Plugin"></a>
# Player Info Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::white_circle:**

PlayerInfo plugin for Thunder framework.

### Table of Contents

- [Introduction](#head.Introduction)
- [Description](#head.Description)
- [Configuration](#head.Configuration)
- [Interfaces](#head.Interfaces)
- [Properties](#head.Properties)
- [Notifications](#head.Notifications)

<a name="head.Introduction"></a>
# Introduction

<a name="head.Scope"></a>
## Scope

This document describes purpose and functionality of the PlayerInfo plugin. It includes detailed specification about its configuration, properties provided and notifications sent.

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

The PlayerInfo plugin helps to get system supported Audio Video codecs.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *PlayerInfo*) |
| classname | string | mandatory | Class name: *PlayerInfo* |
| locator | string | mandatory | Library name: *libWPEPlayerInfo.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- IPlayerProperties ([IPlayerInfo.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IPlayerInfo.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- Dolby::IOutput ([IDolby.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IDolby.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a name="head.Properties"></a>
# Properties

The following properties are provided by the PlayerInfo plugin:

PlayerProperties interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [audiocodecs](#property.audiocodecs) | read-only | Query Audio Codecs List |
| [videocodecs](#property.videocodecs) | read-only | Query Video Codecs List |
| [resolution](#property.resolution) | read-only | Current Video playback resolution |
| [isaudioequivalenceenabled](#property.isaudioequivalenceenabled) | read-only | Checks Loudness Equivalence in platform |

Dolby Output interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [dolbyatmossupported](#property.dolbyatmossupported) / [dolby_atmosmetadata](#property.dolbyatmossupported) | read-only | Atmos capabilities of Sink |
| [dolbysoundmode](#property.dolbysoundmode) / [dolby_soundmode](#property.dolbysoundmode) | read-only | Sound Mode - Mono/Stereo/Surround |
| [dolbyatmosoutput](#property.dolbyatmosoutput) / [dolby_enableatmosoutput](#property.dolbyatmosoutput) | write-only | Enable Atmos Audio Output |
| [dolbymode](#property.dolbymode) / [dolby_mode](#property.dolbymode) | read/write | Dolby Mode |

<a name="property.audiocodecs"></a>
## *audiocodecs [<sup>property</sup>](#head.Properties)*

Provides access to the query Audio Codecs List.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | Query Audio Codecs List |
| result[#] | string | mandatory | *...* (must be one of the following: *AudioAac, AudioAc3, AudioAc3Plus, AudioDts, AudioMpeg1, AudioMpeg2, AudioMpeg3, AudioMpeg4, AudioOpus, AudioUndefined, AudioVorbisOgg, AudioWav*) |

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
    "AudioUndefined"
  ]
}
```

<a name="property.videocodecs"></a>
## *videocodecs [<sup>property</sup>](#head.Properties)*

Provides access to the query Video Codecs List.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | Query Video Codecs List |
| result[#] | string | mandatory | *...* (must be one of the following: *VideoH263, VideoH264, VideoH265, VideoH26510, VideoMpeg, VideoMpeg2, VideoMpeg4, VideoUndefined, VideoVp10, VideoVp8, VideoVp9*) |

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
    "VideoUndefined"
  ]
}
```

<a name="property.resolution"></a>
## *resolution [<sup>property</sup>](#head.Properties)*

Provides access to the current Video playback resolution.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Current Video playback resolution (must be one of the following: *Resolution1080i, Resolution1080i24, Resolution1080i25, Resolution1080i30, Resolution1080i50, Resolution1080p, Resolution1080p24, Resolution1080p25, Resolution1080p30, Resolution1080p50, Resolution2160p, Resolution2160p24, Resolution2160p25, Resolution2160p30, Resolution2160p50, Resolution2160p60, Resolution480i, Resolution480i24, Resolution480i25, Resolution480i30, Resolution480i50, Resolution480p, Resolution480p24, Resolution480p25, Resolution480p30, Resolution480p50, Resolution576i, Resolution576i24, Resolution576i25, Resolution576i30, Resolution576i50, Resolution576p, Resolution576p24, Resolution576p25, Resolution576p30, Resolution576p50, Resolution720p, Resolution720p24, Resolution720p25, Resolution720p30, Resolution720p50, ResolutionUnknown*) |

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
  "result": "ResolutionUnknown"
}
```

<a name="property.isaudioequivalenceenabled"></a>
## *isaudioequivalenceenabled [<sup>property</sup>](#head.Properties)*

Provides access to the checks Loudness Equivalence in platform.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | boolean | mandatory | Checks Loudness Equivalence in platform |

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

<a name="property.dolbyatmossupported"></a>
## *dolbyatmossupported [<sup>property</sup>](#head.Properties)*

Provides access to the atmos capabilities of Sink.

> This property is **read-only**.

> ``dolby_atmosmetadata`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | boolean | mandatory | Atmos capabilities of Sink |

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

<a name="property.dolbysoundmode"></a>
## *dolbysoundmode [<sup>property</sup>](#head.Properties)*

Provides access to the sound Mode - Mono/Stereo/Surround.

> This property is **read-only**.

> ``dolby_soundmode`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Sound Mode - Mono/Stereo/Surround (must be one of the following: *Dolbydigital, Dolbydigitalplus, Mono, Passthru, SoundmodeAuto, Stereo, Surround, Unknown*) |

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
  "result": "Unknown"
}
```

<a name="property.dolbyatmosoutput"></a>
## *dolbyatmosoutput [<sup>property</sup>](#head.Properties)*

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

<a name="property.dolbymode"></a>
## *dolbymode [<sup>property</sup>](#head.Properties)*

Provides access to the dolby Mode.

> ``dolby_mode`` is an alternative name for this property. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Dolby mode type (must be one of the following: *Auto, DigitalAc3, DigitalPassthrough, DigitalPcm, DigitalPlus, Ms12*) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Dolby mode type (must be one of the following: *Auto, DigitalAc3, DigitalPassthrough, DigitalPcm, DigitalPlus, Ms12*) |

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
  "result": "DigitalPcm"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PlayerInfo.1.dolbymode",
  "params": "DigitalPcm"
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

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the PlayerInfo plugin:

Dolby Output interface events:

| Notification | Description |
| :-------- | :-------- |
| [soundmodechanged](#notification.soundmodechanged) / [dolby_audiomodechanged](#notification.soundmodechanged) | Signal audio mode change |

<a name="notification.soundmodechanged"></a>
## *soundmodechanged [<sup>notification</sup>](#head.Notifications)*

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
    "mode": "Unknown",
    "enabled": false
  }
}
```

