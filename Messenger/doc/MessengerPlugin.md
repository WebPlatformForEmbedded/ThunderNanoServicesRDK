<!-- Generated automatically, DO NOT EDIT! -->
<a id="head_Messenger_Plugin"></a>
# Messenger Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

Messenger plugin for Thunder framework.

### Table of Contents

- [Introduction](#head_Introduction)
- [Description](#head_Description)
- [Configuration](#head_Configuration)
- [Interfaces](#head_Interfaces)
- [Methods](#head_Methods)
- [Notifications](#head_Notifications)

<a id="head_Introduction"></a>
# Introduction

<a id="head_Scope"></a>
## Scope

This document describes purpose and functionality of the Messenger plugin. It includes detailed specification about its configuration, methods provided and notifications sent.

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

The Messenger allows exchanging text messages between users gathered in virtual rooms. The rooms are dynamically created and destroyed based on user attendance. Upon joining a room the client receives a unique token (room ID) to be used for sending and receiving the messages.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a id="head_Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *Messenger*) |
| classname | string | mandatory | Class name: *Messenger* |
| locator | string | mandatory | Library name: *libThunderMessenger.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |

<a id="head_Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- JSONRPC::IMessenger ([IMessenger.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IMessenger.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a id="head_Methods"></a>
# Methods

The following methods are provided by the Messenger plugin:

Built-in methods:

| Method | Description |
| :-------- | :-------- |
| [versions](#method_versions) | Retrieves a list of JSON-RPC interfaces offered by this service |
| [exists](#method_exists) | Checks if a JSON-RPC method or property exists |
| [register](#method_register) | Registers for an asynchronous JSON-RPC notification |
| [unregister](#method_unregister) | Unregisters from an asynchronous JSON-RPC notification |

JSONRPC Messenger interface methods:

| Method | Description |
| :-------- | :-------- |
| [join](#method_join) | Joins a messaging room |
| [leave](#method_leave) | Leaves a messaging room |
| [send](#method_send) | Sends a message to a messaging room |

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
  "method": "Messenger.1.versions"
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    {
      "name": "JMyInterface",
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

This method will return *True* for the following methods/properties: *versions, exists, register, unregister, join, leave, send*.

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
  "method": "Messenger.1.exists",
  "params": {
    "method": "versions"
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

This method supports the following event names: *[roomupdate](#notification_roomupdate), [userupdate](#notification_userupdate), [message](#notification_message)*.

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
  "method": "Messenger.1.register",
  "params": {
    "event": "roomupdate",
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

This method supports the following event names: *[roomupdate](#notification_roomupdate), [userupdate](#notification_userupdate), [message](#notification_message)*.

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
  "method": "Messenger.1.unregister",
  "params": {
    "event": "roomupdate",
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

<a id="method_join"></a>
## *join [<sup>method</sup>](#head_Methods)*

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
| params?.acl[#] | string | mandatory | *...* |

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
    "secure": "secure",
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

<a id="method_leave"></a>
## *leave [<sup>method</sup>](#head_Methods)*

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

<a id="method_send"></a>
## *send [<sup>method</sup>](#head_Methods)*

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

<a id="head_Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the Messenger plugin:

JSONRPC Messenger interface events:

| Notification | Description |
| :-------- | :-------- |
| [roomupdate](#notification_roomupdate) | Notifies of room status changes |
| [userupdate](#notification_userupdate) | Notifies of user status changes |
| [message](#notification_message) | Notifies of messages sent the the room |

<a id="notification_roomupdate"></a>
## *roomupdate [<sup>notification</sup>](#head_Notifications)*

Notifies of room status changes.

### Description

Immediately after registering to this notification the listener will sequentially receive updates of all rooms that have been created so far.

> This notification may also be triggered by client registration.

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
    "action": "destroyed",
    "secure": "secure"
  }
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.roomupdate``.

<a id="notification_userupdate"></a>
## *userupdate [<sup>notification</sup>](#head_Notifications)*

Notifies of user status changes.

### Description

Immediately after registering to this notification the listener will sequentially receive updates of all users that have joined the room so far.

> This notification may also be triggered by client registration.

### Parameters

> The *roomId* parameter shall be passed within the *id* parameter to the ``register`` call, i.e. ``<roomid>.<client-id>``.

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
    "action": "left"
  }
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<roomid>.<client-id>.userupdate``.

> The *roomId* parameter is passed within the notification designator, i.e. ``<roomid>.<client-id>.userupdate``.

<a id="notification_message"></a>
## *message [<sup>notification</sup>](#head_Notifications)*

Notifies of messages sent the the room.

### Parameters

> The *roomId* parameter shall be passed within the *id* parameter to the ``register`` call, i.e. ``<roomid>.<client-id>``.

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

> The *client ID* parameter is passed within the notification designator, i.e. ``<roomid>.<client-id>.message``.

> The *roomId* parameter is passed within the notification designator, i.e. ``<roomid>.<client-id>.message``.

