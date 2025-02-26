<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.Location_Sync_Plugin"></a>
# Location Sync Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

LocationSync plugin for Thunder framework.

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

This document describes purpose and functionality of the LocationSync plugin. It includes detailed specification about its configuration, methods and properties as well as sent notifications.

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

The LocationSync plugin provides geo-location functionality.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
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

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- ITimeZone ([ITimeZone.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/ITimeZone.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- ILocationSync ([ILocationSync.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/ILocationSync.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a name="head.Methods"></a>
# Methods

The following methods are provided by the LocationSync plugin:

LocationSync interface methods:

| Method | Description |
| :-------- | :-------- |
| [sync](#method.sync) | Synchronize the location |

<a name="method.sync"></a>
## *sync [<sup>method</sup>](#head.Methods)*

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

<a name="head.Properties"></a>
# Properties

The following properties are provided by the LocationSync plugin:

TimeZone interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [timezone](#property.timezone) | read/write | TimeZone for system |

LocationSync interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [location](#property.location) | read-only | Get information about the location |

<a name="property.timezone"></a>
## *timezone [<sup>property</sup>](#head.Properties)*

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

<a name="property.location"></a>
## *location [<sup>property</sup>](#head.Properties)*

Provides access to the get information about the location.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Get information about the location |
| result?.city | string | optional | City name |
| result?.country | string | optional | Country name |
| result?.region | string | optional | Region name |
| result?.timezone | string | optional | <sup>*(deprecated)*</sup> Time zone information |
| result?.publicip | string | optional | <sup>*(deprecated)*</sup> Public IP |

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

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the LocationSync plugin:

TimeZone interface events:

| Notification | Description |
| :-------- | :-------- |
| [timezonechanged](#notification.timezonechanged) | TimeZone was set for the system |

LocationSync interface events:

| Notification | Description |
| :-------- | :-------- |
| [updated](#notification.updated) / [locationchange](#notification.updated) | Signals a location change |

<a name="notification.timezonechanged"></a>
## *timezonechanged [<sup>notification</sup>](#head.Notifications)*

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

<a name="notification.updated"></a>
## *updated [<sup>notification</sup>](#head.Notifications)*

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

