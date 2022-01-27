<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.MessageControl_Plugin"></a>
# MessageControl Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

A MessageControl plugin for Thunder framework.

### Table of Contents

- [Introduction](#head.Introduction)
- [Description](#head.Description)
- [Configuration](#head.Configuration)
- [Interfaces](#head.Interfaces)
- [Methods](#head.Methods)

<a name="head.Introduction"></a>
# Introduction

<a name="head.Scope"></a>
## Scope

This document describes purpose and functionality of the MessageControl plugin. It includes detailed specification about its configuration and methods provided.

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

The MessageControl plugin allows reading of the traces from WPEFramework, and controlling them tracing and logging. Allows for outputting logging messages to the websocket.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| callsign | string | Plugin instance name (default: *MessageControl*) |
| classname | string | Class name: *MessageControl* |
| locator | string | Library name: *libWPEFrameworkMessageControl.so* |
| autostart | boolean | Determines if the plugin shall be started automatically along with the framework |
| console | boolean | Output messages to the console |
| syslog | boolean | Output messages to the syslog |
| filepath | string | Path to file (inside VolatilePath), where messages can be stored |
| abbreviated | boolean | Should the messages be abbreviated |
| maxexportconnections | number | To how many websockets can messages be outputted |

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- [MessageControl.json](https://github.com/rdkcentral/ThunderInterfaces/tree/master/jsonrpc/MessageControl.json)

<a name="head.Methods"></a>
# Methods

The following methods are provided by the MessageControl plugin:

MessageControl interface methods:

| Method | Description |
| :-------- | :-------- |
| [set](#method.set) | Sets messages |
| [status](#method.status) | Retrieves general information |


<a name="method.set"></a>
## *set [<sup>method</sup>](#head.Methods)*

Sets messages.

### Description

Disables/enables all/select category messages for particular module.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params?.type | string | <sup>*(optional)*</sup> Type of message (must be one of the following: *Tracing*, *Logging*, *Warning_reporting*) |
| params?.module | string | <sup>*(optional)*</sup> Module name |
| params?.category | string | <sup>*(optional)*</sup> Category name |
| params.state | string | State value (must be one of the following: *enabled*, *disabled*, *tristated*) |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | null | Always null |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "MessageControl.1.set",
    "params": {
        "type": "Tracing",
        "module": "Plugin_Monitor",
        "category": "Information",
        "state": "disabled"
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

<a name="method.status"></a>
## *status [<sup>method</sup>](#head.Methods)*

Retrieves general information.

### Description

Retrieves the actual trace status information for targeted module and category, if either category nor module is given, all information is returned. It will retrieves the details about console status and remote address(port and binding address), if these are configured.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string | Type of message (must be one of the following: *Tracing*, *Logging*, *Warning_reporting*) |
| params?.module | string | <sup>*(optional)*</sup> Module name |
| params?.category | string | <sup>*(optional)*</sup> Category name |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.console | boolean | Output message to console (config attribute) |
| result.syslog | boolean | Output message to syslog (config attribute) |
| result.fileNameOutput | string | Filepath where messages are stored |
| result.maxexportconnections | number | To how many websocked should the messages be outputted |
| result.abbreviated | boolean | Should messages be shortened |
| result.messages | array | List of currently announced message controls |
| result.messages[#] | object |  |
| result.messages[#]?.type | string | <sup>*(optional)*</sup> Type of message (must be one of the following: *Tracing*, *Logging*, *Warning_reporting*) |
| result.messages[#]?.module | string | <sup>*(optional)*</sup> Module name |
| result.messages[#]?.category | string | <sup>*(optional)*</sup> Category name |
| result.messages[#].state | string | State value (must be one of the following: *enabled*, *disabled*, *tristated*) |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "MessageControl.1.status"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "console": true,
        "syslog": true,
        "fileNameOutput": "/tmp/MessageControl/messages.log",
        "maxexportconnections": 5,
        "abbreviated": true,
        "messages": [
            {
                "type": "Tracing",
                "module": "Plugin_Monitor",
                "category": "Information",
                "state": "disabled"
            },
            {
                "type": "Logging",
                "module": "SysLog",
                "category": "Startup",
                "state": "enabled"
            }
        ]
    }
}
```

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "MessageControl.1.status",
    "params": {
        "type": "Tracing",
        "module": "Plugin_DeviceInfo",
        "category": "Information"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "messages": [
            {
                "type": "Tracing",
                "module": "Plugin_DeviceInfo",
                "category": "Information",
                "state": "disabled"
            }
        ]
    }
}
```

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "MessageControl.1.status",
    "params": {
        "type": "Tracing",
        "module": "Plugin_DeviceInfo",
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "messages": [
            {
                "type": "Tracing",
                "module": "Plugin_DeviceInfo",
                "category": "Information",
                "state": "disabled"
            },
            {
                "type": "Tracing",
                "module": "Plugin_DeviceInfo",
                "category": "Error",
                "state": "enabled"
            }
        ]
    }
}
```
