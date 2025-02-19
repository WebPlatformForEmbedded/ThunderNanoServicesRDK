<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.Messenger_Plugin"></a>
# Messenger Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

Messenger plugin for Thunder framework.

### Table of Contents

- [Introduction](#head.Introduction)
- [Description](#head.Description)
- [Configuration](#head.Configuration)
- [Interfaces](#head.Interfaces)
- [Methods](#head.Methods)
- [Notifications](#head.Notifications)

<a name="head.Introduction"></a>
# Introduction

<a name="head.Scope"></a>
## Scope

This document describes purpose and functionality of the Messenger plugin. It includes detailed specification about its configuration, methods provided and notifications sent.

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

The Messenger allows exchanging text messages between users gathered in virtual rooms. The rooms are dynamically created and destroyed based on user attendance. Upon joining a room the client receives a unique token (room ID) to be used for sending and receiving the messages.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *Messenger*) |
| classname | string | mandatory | Class name: *Messenger* |
| locator | string | mandatory | Library name: *libThunderMessenger.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- IMessenger ([IMessenger.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IMessenger.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a name="head.Methods"></a>
# Methods

The following methods are provided by the Messenger plugin:

Messenger interface methods:

| Method | Description |
| :-------- | :-------- |
| [join](#method.join) | Joins a messaging room |
| [leave](#method.leave) | Leaves a messaging room |
| [send](#method.send) | Sends a message to a messaging room |

<a name="method.join"></a>
## *join [<sup>method</sup>](#head.Methods)*

Joins a messaging room.

### Description

If the specified room does not exist, then it will be created.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.room | string | mandatory | Name of the room to join |
| params.user | string | mandatory | Name of ther user to join as |
| params?.secure | string | optional | Denotes if the room is secure (by default not secure) (must be one of the following: *insecure, secure*) |
| params?.acl | array | optional | List of URL origins with possible wildcards |
| params?.acl[#] | string | optional | *...* |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Token for accessing the room (unique for a user) |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_ILLEGAL_STATE``` | User name is already taken (i.e. the user has already joined the room) |
| ```ERROR_BAD_REQUEST``` | User name or room name is invalid |
| ```ERROR_PRIVILEGED_REQUEST``` | Room security errors |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Messenger.1.join",
  "params": {
    "room": "Lounge",
    "user": "Bob",
    "secure": "insecure",
    "acl": [
      "..."
    ]
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "1e217990dd1cd4f66124"
}
```

<a name="method.leave"></a>
## *leave [<sup>method</sup>](#head.Methods)*

Leaves a messaging room.

### Description

The room ID becomes invalid after this call. If there are no more users, the room will be destroyed and related resources freed.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.roomid | string | mandatory | Token of the room to leave |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | null | mandatory | Always null |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNKNOWN_KEY``` | The room token is invalid |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Messenger.1.leave",
  "params": {
    "roomid": "1e217990dd1cd4f66124"
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

<a name="method.send"></a>
## *send [<sup>method</sup>](#head.Methods)*

Sends a message to a messaging room.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.roomid | string | mandatory | Token of the room to send the message to |
| params.message | string | mandatory | Contents of the message to send |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | null | mandatory | Always null |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNKNOWN_KEY``` | The room token is invalid |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Messenger.1.send",
  "params": {
    "roomid": "1e217990dd1cd4f66124",
    "message": "Hello!"
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

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the Messenger plugin:

Messenger interface events:

| Notification | Description |
| :-------- | :-------- |
| [roomupdate](#notification.roomupdate) | Notifies of room status changes |
| [userupdate](#notification.userupdate) | Notifies of user status changes |
| [message](#notification.message) | Notifies of messages sent the the room |

<a name="notification.roomupdate"></a>
## *roomupdate [<sup>notification</sup>](#head.Notifications)*

Notifies of room status changes.

### Description

Immediately after registering to this notification the listener will sequentially receive updates of all rooms that have been created so far.

> If applicable, this notification may be sent out during registration, reflecting the current status.

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.room | string | mandatory | Name of the room that has changed its status |
| params.action | string | mandatory | New room status (must be one of the following: *created, destroyed*) |
| params.secure | string | mandatory | Denotes if the room is secure (must be one of the following: *insecure, secure*) |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Messenger.1.register",
  "params": {
    "event": "roomupdate",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.roomupdate",
  "params": {
    "room": "Lounge",
    "action": "created",
    "secure": "insecure"
  }
}
```

<a name="notification.userupdate"></a>
## *userupdate [<sup>notification</sup>](#head.Notifications)*

Notifies of user status changes.

### Description

Immediately after registering to this notification the listener will sequentially receive updates of all users that have joined the room so far.

> If applicable, this notification may be sent out during registration, reflecting the current status.

### Parameters

> The *roomId* parameter shall be passed within the client ID during registration, e.g. *1e217990dd1cd4f66124.myid*

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.user | string | mandatory | Name of the user that has changed its status |
| params.action | string | mandatory | New user status (must be one of the following: *joined, left*) |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Messenger.1.register",
  "params": {
    "event": "userupdate",
    "id": "1e217990dd1cd4f66124.myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "1e217990dd1cd4f66124.myid.userupdate",
  "params": {
    "user": "Bob",
    "action": "joined"
  }
}
```

> The *roomId* parameter is passed within the designator, e.g. *1e217990dd1cd4f66124.myid.userupdate*.

<a name="notification.message"></a>
## *message [<sup>notification</sup>](#head.Notifications)*

Notifies of messages sent the the room.

### Parameters

> The *roomId* parameter shall be passed within the client ID during registration, e.g. *1e217990dd1cd4f66124.myid*

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.user | string | mandatory | Name of the user that has sent the message |
| params.message | string | mandatory | Contents of the sent message |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Messenger.1.register",
  "params": {
    "event": "message",
    "id": "1e217990dd1cd4f66124.myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "1e217990dd1cd4f66124.myid.message",
  "params": {
    "user": "Bob",
    "message": "Hello!"
  }
}
```

> The *roomId* parameter is passed within the designator, e.g. *1e217990dd1cd4f66124.myid.message*.

