<!-- Generated automatically, DO NOT EDIT! -->
<a id="head_Monitor_Plugin"></a>
# Monitor Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

Monitor plugin for Thunder framework.

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

This document describes purpose and functionality of the Monitor plugin. It includes detailed specification about its configuration, methods and properties as well as sent notifications.

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

The Monitor plugin provides a watchdog-like functionality for framework processes.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a id="head_Configuration"></a>
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
| configuration?.observables[#] | object | mandatory | *...* |
| configuration?.observables[#]?.callsign | string | optional | Callsign of the plugin to be monitored |
| configuration?.observables[#]?.memory | integer | optional | Interval(in seconds) for a memory measurement |
| configuration?.observables[#]?.memorylimit | integer | optional | Memory threshold in bytes |
| configuration?.observables[#]?.operational | integer | optional | Interval(in seconds) to check the monitored processes |
| configuration?.observables[#]?.restart | object | optional | Restart limits for failures applying to the plugin |
| configuration?.observables[#]?.restart?.window | integer | optional | Time period(in seconds) within which failures must happen for the limit to be considered crossed |
| configuration?.observables[#]?.restart?.limit | integer | optional | Maximum number or restarts to be attempted |

<a id="head_Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- [Monitor.json](https://github.com/rdkcentral/ThunderInterfaces/blob/master/jsonrpc/Monitor.json) (version 1.0.0) (compliant format)
- IMemoryMonitor ([IMemoryMonitor.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IMemoryMonitor.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a id="head_Methods"></a>
# Methods

The following methods are provided by the Monitor plugin:

Built-in methods:

| Method | Description |
| :-------- | :-------- |
| [versions](#method_versions) | Retrieves a list of JSON-RPC interfaces offered by this service |
| [exists](#method_exists) | Checks if a JSON-RPC method or property exists |
| [register](#method_register) | Registers for an asynchronous JSON-RPC notification |
| [unregister](#method_unregister) | Unregisters from an asynchronous JSON-RPC notification |

Monitor interface methods:

| Method | Description |
| :-------- | :-------- |
| [restartlimits](#method_restartlimits) | Sets new restart limits for a service |
| [resetstats](#method_resetstats) | Resets memory and process statistics for a single service watched by the Monitor |

MemoryMonitor interface methods:

| Method | Description |
| :-------- | :-------- |
| [resetstatistics](#method_resetstatistics) | Resets memory statistics for a given service |

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
  "method": "Monitor.1.versions"
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

This method will return *True* for the following methods/properties: *status, restartinglimits, observables, measurementdata, versions, exists, register, unregister, restartlimits, resetstats, resetstatistics*.

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
  "method": "Monitor.1.exists",
  "params": {
    "method": "status"
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

This method supports the following event names: *[action](#notification_action), [statuschanged](#notification_statuschanged)*.

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
  "method": "Monitor.1.register",
  "params": {
    "event": "action",
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

This method supports the following event names: *[action](#notification_action), [statuschanged](#notification_statuschanged)*.

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
  "method": "Monitor.1.unregister",
  "params": {
    "event": "action",
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

<a id="method_restartlimits"></a>
## *restartlimits [<sup>method</sup>](#head_Methods)*

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

<a id="method_resetstats"></a>
## *resetstats [<sup>method</sup>](#head_Methods)*

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

<a id="method_resetstatistics"></a>
## *resetstatistics [<sup>method</sup>](#head_Methods)*

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

<a id="head_Properties"></a>
# Properties

The following properties are provided by the Monitor plugin:

Monitor interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [status](#property_status) | read-only | Service statistics |

MemoryMonitor interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [restartinglimits](#property_restartinglimits) | read/write | Limits of restarting of a service |
| [observables](#property_observables) | read-only | List of services watched by the Monitor |
| [measurementdata](#property_measurementdata) | read-only | Memory statistics for a given service |

<a id="property_status"></a>
## *status [<sup>property</sup>](#head_Properties)*

Provides access to the service statistics.

> This property is **read-only**.

> The *callsign* parameter shall be passed as the index to the property, i.e. ``status@<callsign>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | If omitted then all observed objects will be returned on read |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | Service statistics |
| (property)[#] | object | mandatory | *...* |
| (property)[#].measurements | object | mandatory | Measurements for the service |
| (property)[#].measurements.resident | object | mandatory | Resident memory measurement |
| (property)[#].measurements.resident.min | integer | mandatory | Minimal value measured |
| (property)[#].measurements.resident.max | integer | mandatory | Maximal value measured |
| (property)[#].measurements.resident.average | integer | mandatory | Average of all measurements |
| (property)[#].measurements.resident.last | integer | mandatory | Last measured value |
| (property)[#].measurements.allocated | object | mandatory | Allocated memory measurement |
| (property)[#].measurements.allocated.min | integer | mandatory | Minimal value measured |
| (property)[#].measurements.allocated.max | integer | mandatory | Maximal value measured |
| (property)[#].measurements.allocated.average | integer | mandatory | Average of all measurements |
| (property)[#].measurements.allocated.last | integer | mandatory | Last measured value |
| (property)[#].measurements.shared | object | mandatory | Shared memory measurement |
| (property)[#].measurements.shared.min | integer | mandatory | Minimal value measured |
| (property)[#].measurements.shared.max | integer | mandatory | Maximal value measured |
| (property)[#].measurements.shared.average | integer | mandatory | Average of all measurements |
| (property)[#].measurements.shared.last | integer | mandatory | Last measured value |
| (property)[#].measurements.process | object | mandatory | Processes measurement |
| (property)[#].measurements.process.min | integer | mandatory | Minimal value measured |
| (property)[#].measurements.process.max | integer | mandatory | Maximal value measured |
| (property)[#].measurements.process.average | integer | mandatory | Average of all measurements |
| (property)[#].measurements.process.last | integer | mandatory | Last measured value |
| (property)[#].measurements.operational | boolean | mandatory | Whether the service is up and running |
| (property)[#].measurements.count | integer | mandatory | Number of measurements |
| (property)[#].observable | string | mandatory | A callsign of the watched service |
| (property)[#].restart | object | mandatory | Restart limits for failures applying to the service |
| (property)[#].restart.limit | integer | mandatory | Maximum number or restarts to be attempted |
| (property)[#].restart.window | integer | mandatory | Time period (in seconds) within which failures must happen for the limit to be considered crossed |

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

<a id="property_restartinglimits"></a>
## *restartinglimits [<sup>property</sup>](#head_Properties)*

Provides access to the limits of restarting of a service.

> The *callsign* parameter shall be passed as the index to the property, i.e. ``restartinglimits@<callsign>``.

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

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | object | mandatory | Limits of restarting of a service |
| (property).limit | integer | mandatory | Maximum number or restarts to be attempted |
| (property).window | integer | mandatory | Time period (in seconds) within which failures must happen for the limit to be considered crossed |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Monitor.1.restartinglimits@WebServer"
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
  "method": "Monitor.1.restartinglimits@WebServer",
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

<a id="property_observables"></a>
## *observables [<sup>property</sup>](#head_Properties)*

Provides access to the list of services watched by the Monitor.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | List of monitored services |
| (property)[#] | string | mandatory | *...* |

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

<a id="property_measurementdata"></a>
## *measurementdata [<sup>property</sup>](#head_Properties)*

Provides access to the memory statistics for a given service.

> This property is **read-only**.

> The *callsign* parameter shall be passed as the index to the property, i.e. ``measurementdata@<callsign>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Callsign of the service |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | object | mandatory | Statistics for the requested callsign |
| (property).resident | object | mandatory | Resident memory measurement |
| (property).resident.min | integer | mandatory | Minimal value measured |
| (property).resident.max | integer | mandatory | Maximal value measured |
| (property).resident.average | integer | mandatory | Average of all measurements |
| (property).resident.last | integer | mandatory | Last measured value |
| (property).allocated | object | mandatory | Allocated memory measurement |
| (property).allocated.min | integer | mandatory | Minimal value measured |
| (property).allocated.max | integer | mandatory | Maximal value measured |
| (property).allocated.average | integer | mandatory | Average of all measurements |
| (property).allocated.last | integer | mandatory | Last measured value |
| (property).shared | object | mandatory | Shared memory measurement |
| (property).shared.min | integer | mandatory | Minimal value measured |
| (property).shared.max | integer | mandatory | Maximal value measured |
| (property).shared.average | integer | mandatory | Average of all measurements |
| (property).shared.last | integer | mandatory | Last measured value |
| (property).process | object | mandatory | Processes measurement |
| (property).process.min | integer | mandatory | Minimal value measured |
| (property).process.max | integer | mandatory | Maximal value measured |
| (property).process.average | integer | mandatory | Average of all measurements |
| (property).process.last | integer | mandatory | Last measured value |
| (property).operational | boolean | mandatory | Whether the service is up and running |
| (property).count | integer | mandatory | Number of measurements |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Monitor.1.measurementdata@WebServer"
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

<a id="head_Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the Monitor plugin:

Monitor interface events:

| Notification | Description |
| :-------- | :-------- |
| [action](#notification_action) | Signals an action taken by the Monitor |

MemoryMonitor interface events:

| Notification | Description |
| :-------- | :-------- |
| [statuschanged](#notification_statuschanged) | Signals an action taken by the Monitor |

<a id="notification_action"></a>
## *action [<sup>notification</sup>](#head_Notifications)*

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

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.action``.

<a id="notification_statuschanged"></a>
## *statuschanged [<sup>notification</sup>](#head_Notifications)*

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

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.statuschanged``.

