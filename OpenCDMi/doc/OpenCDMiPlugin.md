<!-- Generated automatically, DO NOT EDIT! -->
<a id="head_OpenCDMi_Plugin"></a>
# OpenCDMi Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

OCDM plugin for Thunder framework.

### Table of Contents

- [Introduction](#head_Introduction)
- [Configuration](#head_Configuration)
- [Interfaces](#head_Interfaces)
- [Properties](#head_Properties)
- [Notifications](#head_Notifications)

<a id="head_Introduction"></a>
# Introduction

<a id="head_Scope"></a>
## Scope

This document describes purpose and functionality of the OCDM plugin. It includes detailed specification about its configuration, properties provided and notifications sent.

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

<a id="head_Configuration"></a>
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
| configuration?.systems[#] | object | mandatory | System properties |
| configuration?.systems[#]?.name | string | optional | Name |
| configuration?.systems[#]?.designators | array | optional | *...* |
| configuration?.systems[#]?.designators[#] | object | mandatory | Designator |
| configuration?.systems[#]?.designators[#].name | string | mandatory | *...* |

<a id="head_Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- [OCDM.json](https://github.com/rdkcentral/ThunderInterfaces/blob/master/jsonrpc/OCDM.json) (version 1.0.0) (uncompliant-extended format)
- IOpenCDM ([IOCDM.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IOCDM.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a id="head_Properties"></a>
# Properties

The following properties are provided by the OCDM plugin:

OCDM interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [drms](#property_drms) | read-only | Supported DRM systems |
| [keysystems](#property_keysystems) | read-only | DRM key systems |
| [sessions](#property_sessions) | read-only | Active sessions enumerator |

OpenCDM interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [systems](#property_systems) | read-only | Supported DRM systems |
| [designators](#property_designators) | read-only | Designators of a specified DRM system |

<a id="property_drms"></a>
## *drms [<sup>property</sup>](#head_Properties)*

Provides access to the supported DRM systems.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | Supported DRM systems |
| (property)[#] | object | mandatory | *...* |
| (property)[#].name | string | mandatory | Name of the DRM |
| (property)[#].keysystems | array | mandatory | Key system identifier list |
| (property)[#].keysystems[#] | string | mandatory | Identifier of a key system |

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

<a id="property_keysystems"></a>
## *keysystems [<sup>property</sup>](#head_Properties)*

Provides access to the DRM key systems.

> This property is **read-only**.

> The *drm system* parameter shall be passed as the index to the property, i.e. ``keysystems@<drm-system>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| drm-system | string | mandatory | *...* |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | DRM key systems |
| (property)[#] | string | mandatory | Identifier of a key system |

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

<a id="property_sessions"></a>
## *sessions [<sup>property</sup>](#head_Properties)*

Provides access to the active sessions enumerator.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | Active sessions enumerator |
| (property)[#] | object | mandatory | *...* |
| (property)[#].drm | string | mandatory | Name of the DRM system |

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

<a id="property_systems"></a>
## *systems [<sup>property</sup>](#head_Properties)*

Provides access to the supported DRM systems.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | Supported DRM systems |
| (property)[#] | string | mandatory | *...* |

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

<a id="property_designators"></a>
## *designators [<sup>property</sup>](#head_Properties)*

Provides access to the designators of a specified DRM system.

> This property is **read-only**.

> The *keysystem* parameter shall be passed as the index to the property, i.e. ``designators@<keysystem>``.

### Index

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| keysystem | string | mandatory | *...* |

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | Designators of a specified DRM system |
| (property)[#] | string | mandatory | *...* |

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

<a id="head_Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the OCDM plugin:

OCDM interface events:

| Notification | Description |
| :-------- | :-------- |
| [drmalreadyinitialized](#notification_drmalreadyinitialized) | Signals that the specified DRM system could not be initialized because it is already initialized by another process |
| [drminitializationstatus](#notification_drminitializationstatus) | Notifies about DRM initialization status |

<a id="notification_drmalreadyinitialized"></a>
## *drmalreadyinitialized [<sup>notification</sup>](#head_Notifications)*

Signals that the specified DRM system could not be initialized because it is already initialized by another process.

### Description

When this event is received, the application owning given DRM system should release it immediately.

### Notification Parameters

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
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.drmalreadyinitialized",
  "params": {
    "drm": "PlayReady"
  }
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.drmalreadyinitialized``.

<a id="notification_drminitializationstatus"></a>
## *drminitializationstatus [<sup>notification</sup>](#head_Notifications)*

Notifies about DRM initialization status.

### Description

Register to this event to be notified about DRM retrying status busy/failure/success

### Notification Parameters

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
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.drminitializationstatus",
  "params": {
    "status": "SUCCESS",
    "drm": "PlayReady"
  }
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.drminitializationstatus``.

