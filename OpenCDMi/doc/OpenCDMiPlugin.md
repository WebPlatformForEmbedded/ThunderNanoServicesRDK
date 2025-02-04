<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.OpenCDMi_Plugin"></a>
# OpenCDMi Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

OCDM plugin for Thunder framework.

### Table of Contents

- [Introduction](#head.Introduction)
- [Configuration](#head.Configuration)
- [Interfaces](#head.Interfaces)
- [Properties](#head.Properties)
- [Notifications](#head.Notifications)

<a name="head.Introduction"></a>
# Introduction

<a name="head.Scope"></a>
## Scope

This document describes purpose and functionality of the OCDM plugin. It includes detailed specification about its configuration, properties provided and notifications sent.

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

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *OCDM*) |
| classname | string | mandatory | Class name: *OCDM* |
| locator | string | mandatory | Library name: *libThunderOCDM.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |
| configuration | object | optional | *...* |
| configuration?.location | string | optional | Location |
| configuration?.connector | string | optional | Connector |
| configuration?.sharepath | string | optional | Sharepath |
| configuration?.sharesize | string | optional | Sharesize |
| configuration?.systems | array | optional | List of key systems |
| configuration?.systems[#] | object | optional | System properties |
| configuration?.systems[#]?.name | string | optional | Name |
| configuration?.systems[#]?.designators | array | optional | *...* |
| configuration?.systems[#]?.designators[#] | object | optional | Designator |
| configuration?.systems[#]?.designators[#].name | string | mandatory | *...* |

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- [OCDM.json](https://github.com/rdkcentral/ThunderInterfaces/blob/master/jsonrpc/OCDM.json) (version 1.0.0) (uncompliant-extended format)
- IOpenCDM ([IOCDM.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IOCDM.h)) (version 1.0.0) (compliant format)

<a name="head.Properties"></a>
# Properties

The following properties are provided by the OCDM plugin:

OCDM interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [drms](#property.drms) | read-only | Supported DRM systems |
| [keysystems](#property.keysystems) | read-only | DRM key systems |
| [sessions](#property.sessions) | read-only | Active sessions enumerator |

OpenCDM interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [systems](#property.systems) | read-only | Supported DRM systems |
| [designators](#property.designators) | read-only | DRM key systems |

<a name="property.drms"></a>
## *drms [<sup>property</sup>](#head.Properties)*

Provides access to the supported DRM systems.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | Supported DRM systems |
| result[#] | object | mandatory | *...* |
| result[#].name | string | mandatory | Name of the DRM |
| result[#].keysystems | array | mandatory | Key system identifier list |
| result[#].keysystems[#] | string | mandatory | Identifier of a key system |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "OCDM.1.drms"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    {
      "name": "PlayReady",
      "keysystems": [
        "com.microsoft.playready"
      ]
    }
  ]
}
```

<a name="property.keysystems"></a>
## *keysystems [<sup>property</sup>](#head.Properties)*

Provides access to the DRM key systems.

> This property is **read-only**.

> The *drm system* argument shall be passed as the index to the property, e.g. ``OCDM.1.keysystems@<drm-system>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| drm-system | string | mandatory | *...* |

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | DRM key systems |
| result[#] | string | mandatory | Identifier of a key system |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_BAD_REQUEST``` | Invalid DRM name |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "OCDM.1.keysystems@PlayReady"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    "com.microsoft.playready"
  ]
}
```

<a name="property.sessions"></a>
## *sessions [<sup>property</sup>](#head.Properties)*

Provides access to the active sessions enumerator.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | Active sessions enumerator |
| result[#] | object | mandatory | *...* |
| result[#].drm | string | mandatory | Name of the DRM system |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "OCDM.1.sessions"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    {
      "drm": "PlayReady"
    }
  ]
}
```

<a name="property.systems"></a>
## *systems [<sup>property</sup>](#head.Properties)*

Provides access to the supported DRM systems.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | Supported DRM systems |
| result[#] | string | mandatory | *...* |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "OCDM.1.systems"
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

<a name="property.designators"></a>
## *designators [<sup>property</sup>](#head.Properties)*

Provides access to the DRM key systems.

> This property is **read-only**.

> The *keysystem* argument shall be passed as the index to the property, e.g. ``OCDM.1.designators@<keysystem>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| keysystem | string | mandatory | *...* |

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | DRM key systems |
| result[#] | string | mandatory | *...* |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNKNOWN_KEY``` | Invalid DRM name |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "OCDM.1.designators@xyz"
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

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the OCDM plugin:

OCDM interface events:

| Notification | Description |
| :-------- | :-------- |
| [drmalreadyinitialized](#notification.drmalreadyinitialized) | Signals that the specified DRM system could not be initialized because it is already initialized by another process |
| [drminitializationstatus](#notification.drminitializationstatus) | Notifies about DRM initialization status |

<a name="notification.drmalreadyinitialized"></a>
## *drmalreadyinitialized [<sup>notification</sup>](#head.Notifications)*

Signals that the specified DRM system could not be initialized because it is already initialized by another process.

### Description

When this event is received, the application owning given DRM system should release it immediately.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.drm | string | mandatory | Name of the DRM system |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "OCDM.1.register",
  "params": {
    "event": "drmalreadyinitialized",
    "id": "client"
  }
}
```

#### Message

```json
{
  "jsonrpc": "2.0",
  "method": "client.drmalreadyinitialized",
  "params": {
    "drm": "PlayReady"
  }
}
```

<a name="notification.drminitializationstatus"></a>
## *drminitializationstatus [<sup>notification</sup>](#head.Notifications)*

Notifies about DRM initialization status.

### Description

Register to this event to be notified about DRM retrying status busy/failure/success

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.status | string | mandatory | BUSY - drm is used by another process, SUCCESS - recovered from BUSY state after retry, FAILED - not recovered after re-trying from BUSY (must be one of the following: *BUSY, FAILED, SUCCESS*) |
| params.drm | string | mandatory | Name of the DRM system |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "OCDM.1.register",
  "params": {
    "event": "drminitializationstatus",
    "id": "client"
  }
}
```

#### Message

```json
{
  "jsonrpc": "2.0",
  "method": "client.drminitializationstatus",
  "params": {
    "status": "SUCCESS",
    "drm": "PlayReady"
  }
}
```

