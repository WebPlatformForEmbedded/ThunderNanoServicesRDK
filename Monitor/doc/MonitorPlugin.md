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
| <a name="ref.Thunder">[Thunder](https://github.com/WebPlatformForEmbedded/Thunder/blob/master/doc/WPE%20-%20API%20-%20Thunder.docx)</a> | Thunder API Reference |

<a name="head.Description"></a>
# Description

The Monitor plugin provides a watchdog-like functionality for framework processes.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *Monitor*) |
| classname | string | mandatory | Class name: *Monitor* |
| locator | string | mandatory | Library name: *libThunderMonitor.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |
| configuration | object | optional | *...* |
| configuration?.observables | array | optional | List of observable plugin details |
| configuration?.observables[#] | object | optional | *...* |
| configuration?.observables[#]?.callsign | string | optional | Callsign of the plugin to be monitored |
| configuration?.observables[#]?.memory | integer | optional | Interval(in seconds) for a memory measurement |
| configuration?.observables[#]?.memorylimit | integer | optional | Memory threshold in bytes |
| configuration?.observables[#]?.operational | integer | optional | Interval(in seconds) to check the monitored processes |
| configuration?.observables[#]?.restart | object | optional | Restart limits for failures applying to the plugin |
| configuration?.observables[#]?.restart?.window | integer | optional | Time period(in seconds) within which failures must happen for the limit to be considered crossed |
| configuration?.observables[#]?.restart?.limit | integer | optional | Maximum number or restarts to be attempted |

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- [Monitor.json](https://github.com/rdkcentral/ThunderInterfaces/blob/master/jsonrpc/Monitor.json) (version 1.0.0) (compliant format)
- IMemoryMonitor ([IMemoryMonitor.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IMemoryMonitor.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a name="head.Methods"></a>
# Methods

The following methods are provided by the Monitor plugin:

Monitor interface methods:

| Method | Description |
| :-------- | :-------- |
| [restartlimits](#method.restartlimits) | Sets new restart limits for a service |
| [resetstats](#method.resetstats) | Resets memory and process statistics for a single service watched by the Monitor |

MemoryMonitor interface methods:

| Method | Description |
| :-------- | :-------- |
| [resetstatistics](#method.resetstatistics) | Resets memory statistics for a given service |

<a name="method.restartlimits"></a>
## *restartlimits [<sup>method</sup>](#head.Methods)*

Sets new restart limits for a service.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.callsign | string | mandatory | The callsign of a service to reset measurements snapshot of |
| params.restart | object | mandatory | Restart limits for failures applying to the service |
| params.restart.limit | integer | mandatory | Maximum number or restarts to be attempted |
| params.restart.window | integer | mandatory | Time period (in seconds) within which failures must happen for the limit to be considered crossed |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | null | mandatory | Always null (default: *None*) |

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

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.callsign | string | mandatory | The callsign of a service to reset statistics of |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Measurements for the service before reset |
| result.measurements | object | mandatory | Measurements for the service |
| result.measurements.resident | object | mandatory | Resident memory measurement |
| result.measurements.resident.min | integer | mandatory | Minimal value measured |
| result.measurements.resident.max | integer | mandatory | Maximal value measured |
| result.measurements.resident.average | integer | mandatory | Average of all measurements |
| result.measurements.resident.last | integer | mandatory | Last measured value |
| result.measurements.allocated | object | mandatory | Allocated memory measurement |
| result.measurements.allocated.min | integer | mandatory | Minimal value measured |
| result.measurements.allocated.max | integer | mandatory | Maximal value measured |
| result.measurements.allocated.average | integer | mandatory | Average of all measurements |
| result.measurements.allocated.last | integer | mandatory | Last measured value |
| result.measurements.shared | object | mandatory | Shared memory measurement |
| result.measurements.shared.min | integer | mandatory | Minimal value measured |
| result.measurements.shared.max | integer | mandatory | Maximal value measured |
| result.measurements.shared.average | integer | mandatory | Average of all measurements |
| result.measurements.shared.last | integer | mandatory | Last measured value |
| result.measurements.process | object | mandatory | Processes measurement |
| result.measurements.process.min | integer | mandatory | Minimal value measured |
| result.measurements.process.max | integer | mandatory | Maximal value measured |
| result.measurements.process.average | integer | mandatory | Average of all measurements |
| result.measurements.process.last | integer | mandatory | Last measured value |
| result.measurements.operational | boolean | mandatory | Whether the service is up and running |
| result.measurements.count | integer | mandatory | Number of measurements |
| result.observable | string | mandatory | A callsign of the watched service |
| result.restart | object | mandatory | Restart limits for failures applying to the service |
| result.restart.limit | integer | mandatory | Maximum number or restarts to be attempted |
| result.restart.window | integer | mandatory | Time period (in seconds) within which failures must happen for the limit to be considered crossed |

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

<a name="method.resetstatistics"></a>
## *resetstatistics [<sup>method</sup>](#head.Methods)*

Resets memory statistics for a given service.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.callsign | string | mandatory | Callsign of the service |

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
  "method": "Monitor.1.resetstatistics",
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
  "result": null
}
```

<a name="head.Properties"></a>
# Properties

The following properties are provided by the Monitor plugin:

Monitor interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [status](#property.status) | read-only | Service statistics |

MemoryMonitor interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [restartinglimits](#property.restartinglimits) | read/write | Limits of restarting of a service |
| [observables](#property.observables) | read-only | List of services watched by the Monitor |
| [measurementdata](#property.measurementdata) | read-only | Memory statistics for a given service |

<a name="property.status"></a>
## *status [<sup>property</sup>](#head.Properties)*

Provides access to the service statistics.

> This property is **read-only**.

> The *callsign* parameter shall be passed as the index to the property, e.g. ``Monitor.1.status@<callsign>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | If omitted then all observed objects will be returned on read |

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | Service statistics |
| result[#] | object | mandatory | *...* |
| result[#].measurements | object | mandatory | Measurements for the service |
| result[#].measurements.resident | object | mandatory | Resident memory measurement |
| result[#].measurements.resident.min | integer | mandatory | Minimal value measured |
| result[#].measurements.resident.max | integer | mandatory | Maximal value measured |
| result[#].measurements.resident.average | integer | mandatory | Average of all measurements |
| result[#].measurements.resident.last | integer | mandatory | Last measured value |
| result[#].measurements.allocated | object | mandatory | Allocated memory measurement |
| result[#].measurements.allocated.min | integer | mandatory | Minimal value measured |
| result[#].measurements.allocated.max | integer | mandatory | Maximal value measured |
| result[#].measurements.allocated.average | integer | mandatory | Average of all measurements |
| result[#].measurements.allocated.last | integer | mandatory | Last measured value |
| result[#].measurements.shared | object | mandatory | Shared memory measurement |
| result[#].measurements.shared.min | integer | mandatory | Minimal value measured |
| result[#].measurements.shared.max | integer | mandatory | Maximal value measured |
| result[#].measurements.shared.average | integer | mandatory | Average of all measurements |
| result[#].measurements.shared.last | integer | mandatory | Last measured value |
| result[#].measurements.process | object | mandatory | Processes measurement |
| result[#].measurements.process.min | integer | mandatory | Minimal value measured |
| result[#].measurements.process.max | integer | mandatory | Maximal value measured |
| result[#].measurements.process.average | integer | mandatory | Average of all measurements |
| result[#].measurements.process.last | integer | mandatory | Last measured value |
| result[#].measurements.operational | boolean | mandatory | Whether the service is up and running |
| result[#].measurements.count | integer | mandatory | Number of measurements |
| result[#].observable | string | mandatory | A callsign of the watched service |
| result[#].restart | object | mandatory | Restart limits for failures applying to the service |
| result[#].restart.limit | integer | mandatory | Maximum number or restarts to be attempted |
| result[#].restart.window | integer | mandatory | Time period (in seconds) within which failures must happen for the limit to be considered crossed |

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

<a name="property.restartinglimits"></a>
## *restartinglimits [<sup>property</sup>](#head.Properties)*

Provides access to the limits of restarting of a service.

> The *callsign* parameter shall be passed as the index to the property, e.g. ``Monitor.1.restartinglimits@<callsign>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Callsign of the service |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | object | mandatory | Limits of restarting of a service |
| (property).value | object | mandatory | Limits of restarts applying to a given service |
| (property).value.limit | integer | mandatory | Maximum number or restarts to be attempted |
| (property).value.window | integer | mandatory | Time period (in seconds) within which failures must happen for the limit to be considered crossed |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Limits of restarting of a service |
| result.limit | integer | mandatory | Maximum number or restarts to be attempted |
| result.window | integer | mandatory | Time period (in seconds) within which failures must happen for the limit to be considered crossed |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Monitor.1.restartinglimits@xyz"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "limit": 3,
    "window": 60
  }
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Monitor.1.restartinglimits@xyz",
  "params": {
    "value": {
      "limit": 3,
      "window": 60
    }
  }
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

<a name="property.observables"></a>
## *observables [<sup>property</sup>](#head.Properties)*

Provides access to the list of services watched by the Monitor.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | List of monitored services |
| result[#] | string | mandatory | *...* |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Monitor.1.observables"
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

<a name="property.measurementdata"></a>
## *measurementdata [<sup>property</sup>](#head.Properties)*

Provides access to the memory statistics for a given service.

> This property is **read-only**.

> The *callsign* parameter shall be passed as the index to the property, e.g. ``Monitor.1.measurementdata@<callsign>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Callsign of the service |

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Statistics for the requested callsign |
| result.resident | object | mandatory | Resident memory measurement |
| result.resident.min | integer | mandatory | Minimal value measured |
| result.resident.max | integer | mandatory | Maximal value measured |
| result.resident.average | integer | mandatory | Average of all measurements |
| result.resident.last | integer | mandatory | Last measured value |
| result.allocated | object | mandatory | Allocated memory measurement |
| result.allocated.min | integer | mandatory | Minimal value measured |
| result.allocated.max | integer | mandatory | Maximal value measured |
| result.allocated.average | integer | mandatory | Average of all measurements |
| result.allocated.last | integer | mandatory | Last measured value |
| result.shared | object | mandatory | Shared memory measurement |
| result.shared.min | integer | mandatory | Minimal value measured |
| result.shared.max | integer | mandatory | Maximal value measured |
| result.shared.average | integer | mandatory | Average of all measurements |
| result.shared.last | integer | mandatory | Last measured value |
| result.process | object | mandatory | Processes measurement |
| result.process.min | integer | mandatory | Minimal value measured |
| result.process.max | integer | mandatory | Maximal value measured |
| result.process.average | integer | mandatory | Average of all measurements |
| result.process.last | integer | mandatory | Last measured value |
| result.operational | boolean | mandatory | Whether the service is up and running |
| result.count | integer | mandatory | Number of measurements |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Monitor.1.measurementdata@xyz"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
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
  }
}
```

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the Monitor plugin:

Monitor interface events:

| Notification | Description |
| :-------- | :-------- |
| [action](#notification.action) | Signals an action taken by the Monitor |

MemoryMonitor interface events:

| Notification | Description |
| :-------- | :-------- |
| [statuschanged](#notification.statuschanged) | Signals an action taken by the Monitor |

<a name="notification.action"></a>
## *action [<sup>notification</sup>](#head.Notifications)*

Signals an action taken by the Monitor.

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.callsign | string | mandatory | Callsign of the service the Monitor acted upon |
| params.action | string | mandatory | The action executed by the Monitor on a service. One of: "Activate", "Deactivate", "StoppedRestarting" |
| params.reason | string | mandatory | A message describing the reason the action was taken |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Monitor.1.register",
  "params": {
    "event": "action",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.action",
  "params": {
    "callsign": "WebServer",
    "action": "Deactivate",
    "reason": "EXCEEDED_MEMORY"
  }
}
```

<a name="notification.statuschanged"></a>
## *statuschanged [<sup>notification</sup>](#head.Notifications)*

Signals an action taken by the Monitor.

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.callsign | string | mandatory | Callsign of the service the Monitor acted upon |
| params.action | string | mandatory | Ation executed by the Monitor on a service (must be one of the following: *Activated, Deactivated, RestartingStopped*) |
| params?.reason | string | optional | Mssage describing the reason the action was taken (must be one of the following: *ExceededMemory, NotOperational*) |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Monitor.1.register",
  "params": {
    "event": "statuschanged",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.statuschanged",
  "params": {
    "callsign": "WebServer",
    "action": "Deactivate",
    "reason": "EXCEEDED_MEMORY"
  }
}
```

