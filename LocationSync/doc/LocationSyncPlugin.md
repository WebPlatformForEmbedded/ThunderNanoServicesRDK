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

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| callsign | string | Plugin instance name (default: *LocationSync*) |
| classname | string | Class name: *LocationSync* |
| locator | string | Library name: *libWPELocationSync.so* |
| startmode | string | Determines in which state the plugin should be moved to at startup of the framework |
| configuration | object | <sup>*(optional)*</sup> *...* |
| configuration?.interval | integer | <sup>*(optional)*</sup> Maximum time duration between each request to the Location Server (default: 10) |
| configuration?.retries | integer | <sup>*(optional)*</sup> Maximum number of request reties to the Location Server (default:20) |
| configuration?.source | string | <sup>*(optional)*</sup> URI of the Location Server (default:"http://jsonip.metrological.com/?maf=true") |
| configuration?.timezone | string | <sup>*(optional)*</sup> With this the timezone can be overridden, otherwise taken from location (note can als be overriden with JSONRPC call) |

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- ITimeZone ([ITimeZone.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/ITimeZone.h)) (version 1.0.0) (compliant format)
- [LocationSync.json](https://github.com/rdkcentral/ThunderInterfaces/blob/master/jsonrpc/LocationSync.json) (version 1.0.0) (compliant format)

<a name="head.Methods"></a>
# Methods

The following methods are provided by the LocationSync plugin:

LocationSync interface methods:

| Method | Description |
| :-------- | :-------- |
| [sync](#method.sync) | Synchronizes the location |

<a name="method.sync"></a>
## *sync [<sup>method</sup>](#head.Methods)*

Synchronizes the location.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | null | Always null (default: *None*) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_GENERAL``` | Failed to synchdonize the location |
| ```ERROR_UNAVAILABLE``` | Unavailable locator |
| ```ERROR_INCORRECT_URL``` | Incorrect URL |
| ```ERROR_INPROGRESS``` | Probing in progress |

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

| Property | Description |
| :-------- | :-------- |
| [timezone](#property.timezone) | TimeZone for system |

LocationSync interface properties:

| Property | Description |
| :-------- | :-------- |
| [location](#property.location) (read-only) | Location information |

<a name="property.timezone"></a>
## *timezone [<sup>property</sup>](#head.Properties)*

Provides access to the timeZone for system.

### Value

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| (property) | string | TimeZone for system |

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

Provides access to the location information.

> This property is **read-only**.

### Value

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object | Location information |
| result.city | string | City name |
| result.country | string | Country name |
| result.region | string | Region name |
| result.timezone | string | Time zone information |
| result.publicip | string | Public IP |

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
    "region": "Wroclaw",
    "timezone": "CET-1CEST,M3.5.0,M10.5.0/3",
    "publicip": "78.11.117.118"
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
| [locationchange](#notification.locationchange) | Signals a location change |

<a name="notification.timezonechanged"></a>
## *timezonechanged [<sup>notification</sup>](#head.Notifications)*

TimeZone was set for the system.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | string | New TimeZone |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "LocationSync.1.register",
  "params": {
    "event": "timezonechanged",
    "id": "client"
  }
}
```

#### Message

```json
{
  "jsonrpc": "2.0",
  "method": "client.timezonechanged",
  "params": "..."
}
```

<a name="notification.locationchange"></a>
## *locationchange [<sup>notification</sup>](#head.Notifications)*

Signals a location change.

### Parameters

This notification carries no parameters.

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "LocationSync.1.register",
  "params": {
    "event": "locationchange",
    "id": "client"
  }
}
```

#### Message

```json
{
  "jsonrpc": "2.0",
  "method": "client.locationchange"
}
```

