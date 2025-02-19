<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.Display_Info_Plugin"></a>
# Display Info Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

DisplayInfo plugin for Thunder framework.

### Table of Contents

- [Introduction](#head.Introduction)
- [Description](#head.Description)
- [Configuration](#head.Configuration)
- [Interfaces](#head.Interfaces)
- [Methods](#head.Methods)
- [Properties](#head.Properties)
- [Notifications](#head.Notifications)

<a name="head.Introduction"></a>
# Introduction

<a name="head.Scope"></a>
## Scope

This document describes purpose and functionality of the DisplayInfo plugin. It includes detailed specification about its configuration, methods and properties as well as sent notifications.

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

The DisplayInfo plugin allows retrieving of various display-related information.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *DisplayInfo*) |
| classname | string | mandatory | Class name: *DisplayInfo* |
| locator | string | mandatory | Library name: *libThunderDisplayInfo.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- IGraphicsProperties ([IDisplayInfo.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IDisplayInfo.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- IConnectionProperties ([IDisplayInfo.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IDisplayInfo.h)) (version 1.0.0) (uncompliant-extended format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- IHDRProperties ([IDisplayInfo.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IDisplayInfo.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- IDisplayProperties ([IDisplayInfo.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IDisplayInfo.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a name="head.Methods"></a>
# Methods

The following methods are provided by the DisplayInfo plugin:

ConnectionProperties interface methods:

| Method | Description |
| :-------- | :-------- |
| [edid](#method.edid) | TV's Extended Display Identification Data |
| [widthincentimeters](#method.widthincentimeters) | Horizontal size in centimeters |
| [heightincentimeters](#method.heightincentimeters) | Vertical size in centimeters |

<a name="method.edid"></a>
## *edid [<sup>method</sup>](#head.Methods)*

TV's Extended Display Identification Data.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.length | integer | mandatory | Length of EDID byte string |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | *...* |
| result.length | integer | mandatory | Length of EDID byte string |
| result.data | string (base64) | mandatory | EDID byte string |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.edid",
  "params": {
    "length": 0
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "length": 0,
    "data": "..."
  }
}
```

<a name="method.widthincentimeters"></a>
## *widthincentimeters [<sup>method</sup>](#head.Methods)*

Horizontal size in centimeters.

### Parameters

This method takes no parameters.

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | integer | mandatory | Width in cm |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.widthincentimeters"
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": 0
}
```

<a name="method.heightincentimeters"></a>
## *heightincentimeters [<sup>method</sup>](#head.Methods)*

Vertical size in centimeters.

### Parameters

This method takes no parameters.

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | integer | mandatory | *...* |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.heightincentimeters"
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": 0
}
```

<a name="head.Properties"></a>
# Properties

The following properties are provided by the DisplayInfo plugin:

GraphicsProperties interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [totalgpuram](#property.totalgpuram) | read-only | Total GPU DRAM memory (in bytes) |
| [freegpuram](#property.freegpuram) | read-only | Free GPU DRAM memory (in bytes) |

ConnectionProperties interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [isaudiopassthrough](#property.isaudiopassthrough) | read-only | Current audio passthrough status on HDMI |
| [connected](#property.connected) | read-only | Current HDMI connection status |
| [width](#property.width) | read-only | Horizontal resolution of TV |
| [height](#property.height) | read-only | Vertical resolution of TV |
| [verticalfreq](#property.verticalfreq) | read-only | Vertical Frequency |
| [hdcpprotection](#property.hdcpprotection) | read/write | HDCP protocol used for transmission |
| [portname](#property.portname) | read-only | Video output port on the STB used for connection to TV |

HDRProperties interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [tvcapabilities](#property.tvcapabilities) | read-only | HDR formats supported by TV |
| [stbcapabilities](#property.stbcapabilities) | read-only | HDR formats supported by STB |
| [hdrsetting](#property.hdrsetting) | read-only | HDR format in use |

DisplayProperties interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [colorspace](#property.colorspace) | read-only | Provides access to the display's Colour space (chroma subsampling format) |
| [framerate](#property.framerate) | read-only | Provides access to Frame Rate |
| [colourdepth](#property.colourdepth) | read-only | Provides access to display's colour Depth |
| [colorimetry](#property.colorimetry) | read-only | Provides access to display's colorimetry |
| [quantizationrange](#property.quantizationrange) | read-only | Provides access to display's Qauntization Range |
| [eotf](#property.eotf) | read-only | Provides access to display's Electro optical transfer function |

<a name="property.totalgpuram"></a>
## *totalgpuram [<sup>property</sup>](#head.Properties)*

Provides access to the total GPU DRAM memory (in bytes).

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | integer | mandatory | Total GPU DRAM memory (in bytes) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.totalgpuram"
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

<a name="property.freegpuram"></a>
## *freegpuram [<sup>property</sup>](#head.Properties)*

Provides access to the free GPU DRAM memory (in bytes).

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | integer | mandatory | Free GPU DRAM memory (in bytes) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.freegpuram"
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

<a name="property.isaudiopassthrough"></a>
## *isaudiopassthrough [<sup>property</sup>](#head.Properties)*

Provides access to the current audio passthrough status on HDMI.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | boolean | mandatory | Enabled/Disabled |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.isaudiopassthrough"
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

<a name="property.connected"></a>
## *connected [<sup>property</sup>](#head.Properties)*

Provides access to the current HDMI connection status.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | boolean | mandatory | Connected/Disconnected |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.connected"
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

<a name="property.width"></a>
## *width [<sup>property</sup>](#head.Properties)*

Provides access to the horizontal resolution of TV.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | integer | mandatory | Width of TV in pixels |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.width"
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

<a name="property.height"></a>
## *height [<sup>property</sup>](#head.Properties)*

Provides access to the vertical resolution of TV.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | integer | mandatory | Height of TV in pixels |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.height"
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

<a name="property.verticalfreq"></a>
## *verticalfreq [<sup>property</sup>](#head.Properties)*

Provides access to the vertical Frequency.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | integer | mandatory | Vertical freq |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.verticalfreq"
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

<a name="property.hdcpprotection"></a>
## *hdcpprotection [<sup>property</sup>](#head.Properties)*

Provides access to the HDCP protocol used for transmission.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | HDCP protocol used for transmission (must be one of the following: *Hdcp1x, Hdcp2x, HdcpAuto, HdcpUnencrypted*) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Protocol (must be one of the following: *Hdcp1x, Hdcp2x, HdcpAuto, HdcpUnencrypted*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.hdcpprotection"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "HdcpUnencrypted"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.hdcpprotection",
  "params": "HdcpUnencrypted"
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

<a name="property.portname"></a>
## *portname [<sup>property</sup>](#head.Properties)*

Provides access to the video output port on the STB used for connection to TV.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Video output port name |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.portname"
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

<a name="property.tvcapabilities"></a>
## *tvcapabilities [<sup>property</sup>](#head.Properties)*

Provides access to the HDR formats supported by TV.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | HDR formats supported by TV |
| result[#] | string | mandatory | *...* (must be one of the following: *Hdr10, Hdr10plus, HdrDolbyvision, HdrHlg, HdrOff, HdrTechnicolor*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.tvcapabilities"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "HdrOff"
  ]
}
```

<a name="property.stbcapabilities"></a>
## *stbcapabilities [<sup>property</sup>](#head.Properties)*

Provides access to the HDR formats supported by STB.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | HDR formats supported by STB |
| result[#] | string | mandatory | *...* (must be one of the following: *Hdr10, Hdr10plus, HdrDolbyvision, HdrHlg, HdrOff, HdrTechnicolor*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.stbcapabilities"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "HdrOff"
  ]
}
```

<a name="property.hdrsetting"></a>
## *hdrsetting [<sup>property</sup>](#head.Properties)*

Provides access to the HDR format in use.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | HDR format (must be one of the following: *Hdr10, Hdr10plus, HdrDolbyvision, HdrHlg, HdrOff, HdrTechnicolor*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.hdrsetting"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "HdrOff"
}
```

<a name="property.colorspace"></a>
## *colorspace [<sup>property</sup>](#head.Properties)*

Provides access to the provides access to the display's Colour space (chroma subsampling format).

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Colour space (must be one of the following: *FormatOther, FormatRgb444, FormatUnknown, FormatYcbcr420, FormatYcbcr422, FormatYcbcr444*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.colorspace"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "FormatUnknown"
}
```

<a name="property.framerate"></a>
## *framerate [<sup>property</sup>](#head.Properties)*

Provides access to the provides access to Frame Rate.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Frame rate (must be one of the following: *Framerate11988, Framerate120, Framerate144, Framerate23976, Framerate24, Framerate25, Framerate2997, Framerate30, Framerate47952, Framerate48, Framerate50, Framerate5994, Framerate60, FramerateUnknown*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.framerate"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "FramerateUnknown"
}
```

<a name="property.colourdepth"></a>
## *colourdepth [<sup>property</sup>](#head.Properties)*

Provides access to the provides access to display's colour Depth.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Colour depth (must be one of the following: *Colordepth10Bit, Colordepth12Bit, Colordepth8Bit, ColordepthUnknown*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.colourdepth"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "ColordepthUnknown"
}
```

<a name="property.colorimetry"></a>
## *colorimetry [<sup>property</sup>](#head.Properties)*

Provides access to the provides access to display's colorimetry.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | Display colorimetry |
| result[#] | string | mandatory | *...* (must be one of the following: *ColorimetryBt2020rgbYcbcr, ColorimetryBt2020yccbcbrc, ColorimetryBt709, ColorimetryOprgb, ColorimetryOpycc601, ColorimetryOther, ColorimetrySmpte170m, ColorimetrySycc601, ColorimetryUnknown, ColorimetryXvycc601, ColorimetryXvycc709*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.colorimetry"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "ColorimetryUnknown"
  ]
}
```

<a name="property.quantizationrange"></a>
## *quantizationrange [<sup>property</sup>](#head.Properties)*

Provides access to the provides access to display's Qauntization Range.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Quantization range (must be one of the following: *QuantizationrangeFull, QuantizationrangeLimited, QuantizationrangeUnknown*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.quantizationrange"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "QuantizationrangeUnknown"
}
```

<a name="property.eotf"></a>
## *eotf [<sup>property</sup>](#head.Properties)*

Provides access to the provides access to display's Electro optical transfer function.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Display's EOTF (must be one of the following: *EotfBt1886, EotfBt2100, EotfOther, EotfSmpteSt2084, EotfUnknown*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.eotf"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "EotfUnknown"
}
```

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the DisplayInfo plugin:

ConnectionProperties interface events:

| Notification | Description |
| :-------- | :-------- |
| [updated](#notification.updated) | Signal changes on the display update |

<a name="notification.updated"></a>
## *updated [<sup>notification</sup>](#head.Notifications)*

Signal changes on the display update..

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.event | string | mandatory | Source of this event (must be one of the following: *HdcpChange, HdmiChange, PostResolutionChange, PreResolutionChange*) |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.register",
  "params": {
    "event": "updated",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.updated",
  "params": {
    "event": "PreResolutionChange"
  }
}
```

