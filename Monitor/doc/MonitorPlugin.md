<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.Monitor_Plugin"></a>
# Monitor Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

Monitor plugin for Thunder framework.

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

This document describes purpose and functionality of the Monitor plugin. It includes detailed specification about its configuration, methods and properties as well as sent notifications.

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

The Monitor plugin provides a watchdog-like functionality for framework processes.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| callsign | string | Plugin instance name (default: *Monitor*) |
| classname | string | Class name: *Monitor* |
| locator | string | Library name: *libWPEFrameworkMonitor.so* |
| startmode | string | Determines in which state the plugin should be moved to at startup of the framework |
| configuration | object | <sup>*(optional)*</sup>  |
| configuration?.observables | array | <sup>*(optional)*</sup> List of observable plugin details |
| configuration?.observables[#] | object | <sup>*(optional)*</sup>  |
| configuration?.observables[#]?.callsign | string | <sup>*(optional)*</sup> Callsign of the plugin to be monitored |
| configuration?.observables[#]?.memory | integer | <sup>*(optional)*</sup> Interval(in seconds) for a memory measurement |
| configuration?.observables[#]?.memorylimit | integer | <sup>*(optional)*</sup> Memory threshold in bytes |
| configuration?.observables[#]?.operational | integer | <sup>*(optional)*</sup> Interval(in seconds) to check the monitored processes |
| configuration?.observables[#]?.restart | object | <sup>*(optional)*</sup> Restart limits for failures applying to the plugin |
| configuration?.observables[#]?.restart?.window | integer | <sup>*(optional)*</sup> Time period(in seconds) within which failures must happen for the limit to be considered crossed |
| configuration?.observables[#]?.restart?.limit | integer | <sup>*(optional)*</sup> Maximum number or restarts to be attempted |

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- [Monitor.json](https://github.com/rdkcentral/ThunderInterfaces/blob/master/jsonrpc/Monitor.json) (version 1.0.0) (compliant format)

<a name="head.Methods"></a>
# Methods

The following methods are provided by the Monitor plugin:

Monitor interface methods:

| Method | Description |
| :-------- | :-------- |
| [restartlimits](#method.restartlimits) | Sets new restart limits for a service |
| [resetstats](#method.resetstats) | Resets memory and process statistics for a single service watched by the Monitor |

<a name="method.restartlimits"></a>
## *restartlimits [<sup>method</sup>](#head.Methods)*

Sets new restart limits for a service.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.callsign | string | The callsign of a service to reset measurements snapshot of |
| params.restart | object | Restart limits for failures applying to the service |
| params.restart.limit | integer | Maximum number or restarts to be attempted |
| params.restart.window | integer | Time period (in seconds) within which failures must happen for the limit to be considered crossed |

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
  "method": "Monitor.1.restartlimits",
  "params": {
    "callsign": "WebServer",
    "restart": {
      "limit": 3,
      "window": 60
    }
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

<a name="method.resetstats"></a>
## *resetstats [<sup>method</sup>](#head.Methods)*

Resets memory and process statistics for a single service watched by the Monitor.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.callsign | string | The callsign of a service to reset statistics of |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Measurements for the service before reset |
| result.measurements | object | Measurements for the service |
| result.measurements.resident | object | Resident memory measurement |
| result.measurements.resident.min | integer | Minimal value measured |
| result.measurements.resident.max | integer | Maximal value measured |
| result.measurements.resident.average | integer | Average of all measurements |
| result.measurements.resident.last | integer | Last measured value |
| result.measurements.allocated | object | Allocated memory measurement |
| result.measurements.allocated.min | integer | Minimal value measured |
| result.measurements.allocated.max | integer | Maximal value measured |
| result.measurements.allocated.average | integer | Average of all measurements |
| result.measurements.allocated.last | integer | Last measured value |
| result.measurements.shared | object | Shared memory measurement |
| result.measurements.shared.min | integer | Minimal value measured |
| result.measurements.shared.max | integer | Maximal value measured |
| result.measurements.shared.average | integer | Average of all measurements |
| result.measurements.shared.last | integer | Last measured value |
| result.measurements.process | object | Processes measurement |
| result.measurements.process.min | integer | Minimal value measured |
| result.measurements.process.max | integer | Maximal value measured |
| result.measurements.process.average | integer | Average of all measurements |
| result.measurements.process.last | integer | Last measured value |
| result.measurements.operational | boolean | Whether the service is up and running |
| result.measurements.count | integer | Number of measurements |
| result.observable | string | A callsign of the watched service |
| result.restart | object | Restart limits for failures applying to the service |
| result.restart.limit | integer | Maximum number or restarts to be attempted |
| result.restart.window | integer | Time period (in seconds) within which failures must happen for the limit to be considered crossed |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Monitor.1.resetstats",
  "params": {
    "callsign": "WebServer"
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "measurements": {
      "resident": {
        "min": 0,
        "max": 100,
        "average": 50,
        "last": 100
      },
      "allocated": {
        "min": 0,
        "max": 100,
        "average": 50,
        "last": 100
      },
      "shared": {
        "min": 0,
        "max": 100,
        "average": 50,
        "last": 100
      },
      "process": {
        "min": 0,
        "max": 100,
        "average": 50,
        "last": 100
      },
      "operational": true,
      "count": 100
    },
    "observable": "callsign",
    "restart": {
      "limit": 3,
      "window": 60
    }
  }
}
```

<a name="head.Properties"></a>
# Properties

The following properties are provided by the Monitor plugin:

Monitor interface properties:

| Property | Description |
| :-------- | :-------- |
| [status](#property.status) (read-only) | Service statistics |

<a name="property.status"></a>
## *status [<sup>property</sup>](#head.Properties)*

Provides access to the service statistics.

> This property is **read-only**.

### Value

> The *callsign* argument shall be passed as the index to the property, e.g. ``Monitor.1.status@WebServer``. If omitted then all observed objects will be returned on read.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | array | Service statistics |
| result[#] | object |  |
| result[#].measurements | object | Measurements for the service |
| result[#].measurements.resident | object | Resident memory measurement |
| result[#].measurements.resident.min | integer | Minimal value measured |
| result[#].measurements.resident.max | integer | Maximal value measured |
| result[#].measurements.resident.average | integer | Average of all measurements |
| result[#].measurements.resident.last | integer | Last measured value |
| result[#].measurements.allocated | object | Allocated memory measurement |
| result[#].measurements.allocated.min | integer | Minimal value measured |
| result[#].measurements.allocated.max | integer | Maximal value measured |
| result[#].measurements.allocated.average | integer | Average of all measurements |
| result[#].measurements.allocated.last | integer | Last measured value |
| result[#].measurements.shared | object | Shared memory measurement |
| result[#].measurements.shared.min | integer | Minimal value measured |
| result[#].measurements.shared.max | integer | Maximal value measured |
| result[#].measurements.shared.average | integer | Average of all measurements |
| result[#].measurements.shared.last | integer | Last measured value |
| result[#].measurements.process | object | Processes measurement |
| result[#].measurements.process.min | integer | Minimal value measured |
| result[#].measurements.process.max | integer | Maximal value measured |
| result[#].measurements.process.average | integer | Average of all measurements |
| result[#].measurements.process.last | integer | Last measured value |
| result[#].measurements.operational | boolean | Whether the service is up and running |
| result[#].measurements.count | integer | Number of measurements |
| result[#].observable | string | A callsign of the watched service |
| result[#].restart | object | Restart limits for failures applying to the service |
| result[#].restart.limit | integer | Maximum number or restarts to be attempted |
| result[#].restart.window | integer | Time period (in seconds) within which failures must happen for the limit to be considered crossed |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Monitor.1.status@WebServer"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    {
      "measurements": {
        "resident": {
          "min": 0,
          "max": 100,
          "average": 50,
          "last": 100
        },
        "allocated": {
          "min": 0,
          "max": 100,
          "average": 50,
          "last": 100
        },
        "shared": {
          "min": 0,
          "max": 100,
          "average": 50,
          "last": 100
        },
        "process": {
          "min": 0,
          "max": 100,
          "average": 50,
          "last": 100
        },
        "operational": true,
        "count": 100
      },
      "observable": "callsign",
      "restart": {
        "limit": 3,
        "window": 60
      }
    }
  ]
}
```

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the Monitor plugin:

Monitor interface events:

| Event | Description |
| :-------- | :-------- |
| [action](#event.action) | Signals an action taken by the Monitor |

<a name="event.action"></a>
## *action [<sup>event</sup>](#head.Notifications)*

Signals an action taken by the Monitor.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.callsign | string | Callsign of the service the Monitor acted upon |
| params.action | string | The action executed by the Monitor on a service. One of: "Activate", "Deactivate", "StoppedRestarting" |
| params.reason | string | A message describing the reason the action was taken |

### Example

```json
{
  "jsonrpc": "2.0",
  "method": "client.events.1.action",
  "params": {
    "callsign": "WebServer",
    "action": "Deactivate",
    "reason": "EXCEEDED_MEMORY"
  }
}
```

