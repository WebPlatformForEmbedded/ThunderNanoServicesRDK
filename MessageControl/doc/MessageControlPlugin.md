<!-- Generated automatically, DO NOT EDIT! -->
<a id="head_MessageControl_Plugin"></a>
# MessageControl Plugin

**Version: 1.0**

**Status: :black_circle::white_circle::white_circle:**

MessageControl plugin for Thunder framework.

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

This document describes purpose and functionality of the MessageControl plugin. It includes detailed specification about its configuration, methods and properties provided.

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

The MessageControl plugin allows reading of the traces from Thunder, and controlling them tracing and logging. Allows for outputting logging messages to the websocket.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a id="head_Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *MessageControl*) |
| classname | string | mandatory | Class name: *MessageControl* |
| locator | string | mandatory | Library name: *libThunderMessageControl.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |
| console | boolean | optional | Enables message output messages to the console |
| syslog | boolean | optional | Enables message ouutput to syslog |
| filepath | string | optional | Path to file (inside VolatilePath) where messages will be stored |
| abbreviated | boolean | optional | Denotes if the messages should be abbreviated |
| maxexportconnections | integer | optional | Specifies to how many websockets can the messages be outputted |
| remote | object | optional | *...* |
| remote.port | integer | mandatory | Port |
| remote?.bindig | string | optional | Binding address |
| remote.interface | string | mandatory | Interface |

<a id="head_Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- IMessageControl ([IMessageControl.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IMessageControl.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a id="head_Methods"></a>
# Methods

The following methods are provided by the MessageControl plugin:

Built-in methods:

| Method | Description |
| :-------- | :-------- |
| [versions](#method_versions) | Retrieves a list of JSON-RPC interfaces offered by this service |
| [exists](#method_exists) | Checks if a JSON-RPC method or property exists |

MessageControl interface methods:

| Method | Description |
| :-------- | :-------- |
| [enable](#method_enable) | Enables/disables a message control |

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
  "method": "MessageControl.1.versions"
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

This method will return *True* for the following methods/properties: *modules, controls, versions, exists, enable*.

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
  "method": "MessageControl.1.exists",
  "params": {
    "method": "modules"
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

<a id="method_enable"></a>
## *enable [<sup>method</sup>](#head_Methods)*

Enables/disables a message control.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.type | string | mandatory | Message type (must be one of the following: *Assert, Invalid, Logging, OperationalStream, Reporting, Tracing*) |
| params.category | string | mandatory | Name of the message category |
| params.module | string | mandatory | Name of the module the message is originating from |
| params.enabled | boolean | mandatory | Denotes if control should be enabled (true) or disabled (false) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | null | mandatory | Always null |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "MessageControl.1.enable",
  "params": {
    "type": "Tracing",
    "category": "Information",
    "module": "Plugin_BluetoothControl",
    "enabled": false
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

The following properties are provided by the MessageControl plugin:

MessageControl interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [modules](#property_modules) | read-only | Retrieves a list of current message modules |
| [controls](#property_controls) | read-only | Retrieves a list of current message controls for a specific module |

<a id="property_modules"></a>
## *modules [<sup>property</sup>](#head_Properties)*

Provides access to the retrieves a list of current message modules.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | Retrieves a list of current message modules |
| (property)[#] | string | mandatory | *...* |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "MessageControl.1.modules"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "..."
  ]
}
```

<a id="property_controls"></a>
## *controls [<sup>property</sup>](#head_Properties)*

Provides access to the retrieves a list of current message controls for a specific module.

> This property is **read-only**.

> The *module* parameter shall be passed as the index to the property, i.e. ``controls@<module>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| module | string | mandatory | *...* |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | Retrieves a list of current message controls for a specific module |
| (property)[#] | object | mandatory | *...* |
| (property)[#].type | string | mandatory | Type of message (must be one of the following: *Assert, Invalid, Logging, OperationalStream, Reporting, Tracing*) |
| (property)[#].category | string | mandatory | Name of the message category |
| (property)[#].module | string | mandatory | Name of the module the message is originating from |
| (property)[#].enabled | boolean | mandatory | Denotes if the control is enabled (true) or disabled (false) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "MessageControl.1.controls@xyz"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    {
      "type": "Tracing",
      "category": "Information",
      "module": "Plugin_BluetoothControl",
      "enabled": false
    }
  ]
}
```

