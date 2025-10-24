<!-- Generated automatically, DO NOT EDIT! -->
<a id="head_Device_Identification_Plugin"></a>
# Device Identification Plugin

**Version: 1.0.0**

**Status: :black_circle::white_circle::white_circle:**

DeviceIdentification interface for Thunder framework.

(Defined with IDeviceIdentification in [IDeviceIdentification.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IDeviceIdentification.h))

### Table of Contents

- [Introduction](#head_Introduction)
- [Description](#head_Description)
- [Methods](#head_Methods)
- [Properties](#head_Properties)

<a id="head_Introduction"></a>
# Introduction

<a id="head_Scope"></a>
## Scope

This document describes purpose and functionality of the DeviceIdentification interface (version 1.0.0). It includes detailed specification about its methods and properties provided.

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

DeviceIdentification JSON-RPC interface.

<a id="head_Methods"></a>
# Methods

The following methods are provided by the DeviceIdentification interface:

Built-in methods:

| Method | Description |
| :-------- | :-------- |
| [exists](#method_exists) | Checks if a JSON-RPC method or property exists |

<a id="method_exists"></a>
## *exists [<sup>method</sup>](#head_Methods)*

Checks if a JSON-RPC method or property exists.

### Description

This method will return *True* for the following methods/properties: *deviceidentification, exists*.

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
  "method": "<callsign>.1.exists",
  "params": {
    "method": "deviceidentification"
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

<a id="head_Properties"></a>
# Properties

The following properties are provided by the DeviceIdentification interface:

DeviceIdentification interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [deviceidentification](#property_deviceidentification) | read-only | Get device paltform specific information |

<a id="property_deviceidentification"></a>
## *deviceidentification [<sup>property</sup>](#head_Properties)*

Provides access to the get device paltform specific information.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | object | mandatory | Get device paltform specific information |
| (property).deviceid | string | mandatory | Device ID |
| (property)?.firmwareversion | string | optional | Version of the device firmware |
| (property)?.chipset | string | optional | Chipset used for this device |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNAVAILABLE``` | Identification not availbale |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "<callsign>.1.deviceidentification"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "deviceid": "WPEuCfrLF45",
    "firmwareversion": "1.0.0",
    "chipset": "BCM2711"
  }
}
```

