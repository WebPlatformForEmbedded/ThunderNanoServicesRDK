<!-- Generated automatically, DO NOT EDIT! -->
<a id="head_Display_Info_Plugin"></a>
# Display Info Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

DisplayInfo plugin for Thunder framework.

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

This document describes purpose and functionality of the DisplayInfo plugin. It includes detailed specification about its configuration, methods and properties as well as sent notifications.

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

The DisplayInfo plugin allows retrieving of various display-related information.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a id="head_Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *DisplayInfo*) |
| classname | string | mandatory | Class name: *DisplayInfo* |
| locator | string | mandatory | Library name: *libThunderDisplayInfo.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |

<a id="head_Interfaces"></a>
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

<a id="head_Methods"></a>
# Methods

The following methods are provided by the DisplayInfo plugin:

Built-in methods:

| Method | Description |
| :-------- | :-------- |
| [versions](#method_versions) | Retrieves a list of JSON-RPC interfaces offered by this service |
| [exists](#method_exists) | Checks if a JSON-RPC method or property exists |
| [register](#method_register) | Registers for an asynchronous JSON-RPC notification |
| [unregister](#method_unregister) | Unregisters from an asynchronous JSON-RPC notification |

ConnectionProperties interface methods:

| Method | Description |
| :-------- | :-------- |
| [edid](#method_edid) | TV's Extended Display Identification Data |
| [widthincentimeters](#method_widthincentimeters) | Horizontal size in centimeters |
| [heightincentimeters](#method_heightincentimeters) | Vertical size in centimeters |

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
  "method": "DisplayInfo.1.versions"
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    {
      "name": "JMyInterface",
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

This method will return *True* for the following methods/properties: *totalgpuram, freegpuram, isaudiopassthrough, connected, width, height, verticalfreq, hdcpprotection, portname, tvcapabilities, stbcapabilities, hdrsetting, colorspace, framerate, colourdepth, colorimetry, quantizationrange, eotf, versions, exists, register, unregister, edid, widthincentimeters, heightincentimeters*.

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
  "method": "DisplayInfo.1.exists",
  "params": {
    "method": "totalgpuram"
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

This method supports the following event names: *[updated](#notification_updated)*.

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
  "method": "DisplayInfo.1.register",
  "params": {
    "event": "updated",
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

This method supports the following event names: *[updated](#notification_updated)*.

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
  "method": "DisplayInfo.1.unregister",
  "params": {
    "event": "updated",
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

<a id="method_edid"></a>
## *edid [<sup>method</sup>](#head_Methods)*

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

<a id="method_widthincentimeters"></a>
## *widthincentimeters [<sup>method</sup>](#head_Methods)*

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

<a id="method_heightincentimeters"></a>
## *heightincentimeters [<sup>method</sup>](#head_Methods)*

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

<a id="head_Properties"></a>
# Properties

The following properties are provided by the DisplayInfo plugin:

GraphicsProperties interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [totalgpuram](#property_totalgpuram) | read-only | Total GPU DRAM memory (in bytes) |
| [freegpuram](#property_freegpuram) | read-only | Free GPU DRAM memory (in bytes) |

ConnectionProperties interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [isaudiopassthrough](#property_isaudiopassthrough) | read-only | Current audio passthrough status on HDMI |
| [connected](#property_connected) | read-only | Current HDMI connection status |
| [width](#property_width) | read-only | Horizontal resolution of TV |
| [height](#property_height) | read-only | Vertical resolution of TV |
| [verticalfreq](#property_verticalfreq) | read-only | Vertical Frequency |
| [hdcpprotection](#property_hdcpprotection) | read/write | HDCP protocol used for transmission |
| [portname](#property_portname) | read-only | Video output port on the STB used for connection to TV |

HDRProperties interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [tvcapabilities](#property_tvcapabilities) | read-only | HDR formats supported by TV |
| [stbcapabilities](#property_stbcapabilities) | read-only | HDR formats supported by STB |
| [hdrsetting](#property_hdrsetting) | read-only | HDR format in use |

DisplayProperties interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [colorspace](#property_colorspace) | read-only | Provides access to the display's Colour space (chroma subsampling format) |
| [framerate](#property_framerate) | read-only | Provides access to Frame Rate |
| [colourdepth](#property_colourdepth) | read-only | Provides access to display's colour Depth |
| [colorimetry](#property_colorimetry) | read-only | Provides access to display's colorimetry |
| [quantizationrange](#property_quantizationrange) | read-only | Provides access to display's Qauntization Range |
| [eotf](#property_eotf) | read-only | Provides access to display's Electro optical transfer function |

<a id="property_totalgpuram"></a>
## *totalgpuram [<sup>property</sup>](#head_Properties)*

Provides access to the total GPU DRAM memory (in bytes).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Total GPU DRAM memory (in bytes) |

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

<a id="property_freegpuram"></a>
## *freegpuram [<sup>property</sup>](#head_Properties)*

Provides access to the free GPU DRAM memory (in bytes).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Free GPU DRAM memory (in bytes) |

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

<a id="property_isaudiopassthrough"></a>
## *isaudiopassthrough [<sup>property</sup>](#head_Properties)*

Provides access to the current audio passthrough status on HDMI.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | boolean | mandatory | Enabled/Disabled |

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

<a id="property_connected"></a>
## *connected [<sup>property</sup>](#head_Properties)*

Provides access to the current HDMI connection status.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | boolean | mandatory | Connected/Disconnected |

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

<a id="property_width"></a>
## *width [<sup>property</sup>](#head_Properties)*

Provides access to the horizontal resolution of TV.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Width of TV in pixels |

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

<a id="property_height"></a>
## *height [<sup>property</sup>](#head_Properties)*

Provides access to the vertical resolution of TV.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Height of TV in pixels |

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

<a id="property_verticalfreq"></a>
## *verticalfreq [<sup>property</sup>](#head_Properties)*

Provides access to the vertical Frequency.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Vertical freq |

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

<a id="property_hdcpprotection"></a>
## *hdcpprotection [<sup>property</sup>](#head_Properties)*

Provides access to the HDCP protocol used for transmission.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | HDCP protocol used for transmission (must be one of the following: *Hdcp1x, Hdcp2x, HdcpAuto, HdcpUnencrypted*) |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Protocol (must be one of the following: *Hdcp1x, Hdcp2x, HdcpAuto, HdcpUnencrypted*) |

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
  "result": "Hdcp1x"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "DisplayInfo.1.hdcpprotection",
  "params": "Hdcp1x"
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

<a id="property_portname"></a>
## *portname [<sup>property</sup>](#head_Properties)*

Provides access to the video output port on the STB used for connection to TV.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Video output port name |

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

<a id="property_tvcapabilities"></a>
## *tvcapabilities [<sup>property</sup>](#head_Properties)*

Provides access to the HDR formats supported by TV.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | HDR formats supported by TV |
| (property)[#] | string | mandatory | *...* (must be one of the following: *Hdr10, Hdr10plus, HdrDolbyvision, HdrHlg, HdrOff, HdrTechnicolor*) |

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
    "Hdr10"
  ]
}
```

<a id="property_stbcapabilities"></a>
## *stbcapabilities [<sup>property</sup>](#head_Properties)*

Provides access to the HDR formats supported by STB.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | HDR formats supported by STB |
| (property)[#] | string | mandatory | *...* (must be one of the following: *Hdr10, Hdr10plus, HdrDolbyvision, HdrHlg, HdrOff, HdrTechnicolor*) |

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
    "Hdr10"
  ]
}
```

<a id="property_hdrsetting"></a>
## *hdrsetting [<sup>property</sup>](#head_Properties)*

Provides access to the HDR format in use.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | HDR format (must be one of the following: *Hdr10, Hdr10plus, HdrDolbyvision, HdrHlg, HdrOff, HdrTechnicolor*) |

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
  "result": "Hdr10"
}
```

<a id="property_colorspace"></a>
## *colorspace [<sup>property</sup>](#head_Properties)*

Provides access to the provides access to the display's Colour space (chroma subsampling format).

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Colour space (must be one of the following: *FormatOther, FormatRgb444, FormatUnknown, FormatYcbcr420, FormatYcbcr422, FormatYcbcr444*) |

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
  "result": "FormatOther"
}
```

<a id="property_framerate"></a>
## *framerate [<sup>property</sup>](#head_Properties)*

Provides access to the provides access to Frame Rate.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Frame rate (must be one of the following: *Framerate11988, Framerate120, Framerate144, Framerate23976, Framerate24, Framerate25, Framerate2997, Framerate30, Framerate47952, Framerate48, Framerate50, Framerate5994, Framerate60, FramerateUnknown*) |

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
  "result": "Framerate23976"
}
```

<a id="property_colourdepth"></a>
## *colourdepth [<sup>property</sup>](#head_Properties)*

Provides access to the provides access to display's colour Depth.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Colour depth (must be one of the following: *Colordepth10Bit, Colordepth12Bit, Colordepth8Bit, ColordepthUnknown*) |

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
  "result": "Colordepth8Bit"
}
```

<a id="property_colorimetry"></a>
## *colorimetry [<sup>property</sup>](#head_Properties)*

Provides access to the provides access to display's colorimetry.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | Display colorimetry |
| (property)[#] | string | mandatory | *...* (must be one of the following: *ColorimetryBt2020rgbYcbcr, ColorimetryBt2020yccbcbrc, ColorimetryBt709, ColorimetryOprgb, ColorimetryOpycc601, ColorimetryOther, ColorimetrySmpte170m, ColorimetrySycc601, ColorimetryUnknown, ColorimetryXvycc601, ColorimetryXvycc709*) |

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
    "ColorimetryOther"
  ]
}
```

<a id="property_quantizationrange"></a>
## *quantizationrange [<sup>property</sup>](#head_Properties)*

Provides access to the provides access to display's Qauntization Range.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Quantization range (must be one of the following: *QuantizationrangeFull, QuantizationrangeLimited, QuantizationrangeUnknown*) |

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
  "result": "QuantizationrangeLimited"
}
```

<a id="property_eotf"></a>
## *eotf [<sup>property</sup>](#head_Properties)*

Provides access to the provides access to display's Electro optical transfer function.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Display's EOTF (must be one of the following: *EotfBt1886, EotfBt2100, EotfOther, EotfSmpteSt2084, EotfUnknown*) |

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
  "result": "EotfOther"
}
```

<a id="head_Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the DisplayInfo plugin:

ConnectionProperties interface events:

| Notification | Description |
| :-------- | :-------- |
| [updated](#notification_updated) | Signal changes on the display update |

<a id="notification_updated"></a>
## *updated [<sup>notification</sup>](#head_Notifications)*

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
    "event": "PostResolutionChange"
  }
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.updated``.

