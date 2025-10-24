<!-- Generated automatically, DO NOT EDIT! -->
<a id="head_Location_Sync_Plugin"></a>
# Location Sync Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

LocationSync plugin for Thunder framework.

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

This document describes purpose and functionality of the LocationSync plugin. It includes detailed specification about its configuration, methods and properties as well as sent notifications.

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

The LocationSync plugin provides geo-location functionality.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a id="head_Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *LocationSync*) |
| classname | string | mandatory | Class name: *LocationSync* |
| locator | string | mandatory | Library name: *libWPELocationSync.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |
| configuration | object | optional | *...* |
| configuration?.interval | integer | optional | Maximum time duration between each request to the Location Server (default: 10) |
| configuration?.retries | integer | optional | Maximum number of request reties to the Location Server (default:20) |
| configuration?.source | string | optional | URI of the Location Server (default:"http://jsonip.metrological.com/?maf=true") |
| configuration?.timezone | string | optional | With this the timezone can be overridden, otherwise taken from location (note can als be overriden with JSONRPC call) |

<a id="head_Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- ITimeZone ([ITimeZone.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/ITimeZone.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- ILocationSync ([ILocationSync.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/ILocationSync.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a id="head_Methods"></a>
# Methods

The following methods are provided by the LocationSync plugin:

Built-in methods:

| Method | Description |
| :-------- | :-------- |
| [versions](#method_versions) | Retrieves a list of JSON-RPC interfaces offered by this service |
| [exists](#method_exists) | Checks if a JSON-RPC method or property exists |
| [register](#method_register) | Registers for an asynchronous JSON-RPC notification |
| [unregister](#method_unregister) | Unregisters from an asynchronous JSON-RPC notification |

LocationSync interface methods:

| Method | Description |
| :-------- | :-------- |
| [sync](#method_sync) | Synchronize the location |

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
  "method": "LocationSync.1.versions"
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

This method will return *True* for the following methods/properties: *timezone, location, versions, exists, register, unregister, sync*.

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
  "method": "LocationSync.1.exists",
  "params": {
    "method": "timezone"
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

This method supports the following event names: *[timezonechanged](#notification_timezonechanged), [updated](#notification_updated)*.

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
  "method": "LocationSync.1.register",
  "params": {
    "event": "timezonechanged",
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

This method supports the following event names: *[timezonechanged](#notification_timezonechanged), [updated](#notification_updated)*.

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
  "method": "LocationSync.1.unregister",
  "params": {
    "event": "timezonechanged",
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

<a id="method_sync"></a>
## *sync [<sup>method</sup>](#head_Methods)*

Synchronize the location.

### Parameters

This method takes no parameters.

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | null | mandatory | Always null |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | Failed to synchdonize the location |
| ```ERROR_UNAVAILABLE``` | Locator is not available |
| ```ERROR_INCORRECT_URL``` | The URL is incorrect |
| ```ERROR_INPROGRESS``` | Probing is still in progress |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "LocationSync.1.sync"
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

The following properties are provided by the LocationSync plugin:

TimeZone interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [timezone](#property_timezone) | read/write | TimeZone for system |

LocationSync interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [location](#property_location) | read-only | Get information about the location |

<a id="property_timezone"></a>
## *timezone [<sup>property</sup>](#head_Properties)*

Provides access to the timeZone for system.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | TimeZone for system |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "LocationSync.1.timezone"
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

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "LocationSync.1.timezone",
  "params": "..."
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

<a id="property_location"></a>
## *location [<sup>property</sup>](#head_Properties)*

Provides access to the get information about the location.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | object | mandatory | Get information about the location |
| (property)?.city | string | optional | City name |
| (property)?.country | string | optional | Country name |
| (property)?.region | string | optional | Region name |
| (property)?.timezone | string | optional | <sup>*(deprecated)*</sup> Time zone information |
| (property)?.publicip | string | optional | <sup>*(deprecated)*</sup> Public IP |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNAVAILABLE``` | Either the internet or the location information is not available |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "LocationSync.1.location"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "city": "Wroclaw",
    "country": "Poland",
    "region": "Silesia"
  }
}
```

<a id="head_Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the LocationSync plugin:

TimeZone interface events:

| Notification | Description |
| :-------- | :-------- |
| [timezonechanged](#notification_timezonechanged) | TimeZone was set for the system |

LocationSync interface events:

| Notification | Description |
| :-------- | :-------- |
| [updated](#notification_updated) / [locationchange](#notification_updated) | Signals a location change |

<a id="notification_timezonechanged"></a>
## *timezonechanged [<sup>notification</sup>](#head_Notifications)*

TimeZone was set for the system.

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | string | mandatory | New TimeZone |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "LocationSync.1.register",
  "params": {
    "event": "timezonechanged",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.timezonechanged",
  "params": "..."
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.timezonechanged``.

<a id="notification_updated"></a>
## *updated [<sup>notification</sup>](#head_Notifications)*

Signals a location change.

> ``locationchange`` is an alternative name for this notification. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Notification Parameters

This notification carries no parameters.

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "LocationSync.1.register",
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
  "method": "myid.updated"
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.updated``.

