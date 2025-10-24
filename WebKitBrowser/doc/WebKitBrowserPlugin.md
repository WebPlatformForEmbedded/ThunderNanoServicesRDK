<!-- Generated automatically, DO NOT EDIT! -->
<a id="head_WebKit_Browser_Plugin"></a>
# WebKit Browser Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

WebKitBrowser plugin for Thunder framework.

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

This document describes purpose and functionality of the WebKitBrowser plugin. It includes detailed specification about its configuration, methods and properties as well as sent notifications.

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

The WebKitBrowser plugin provides web browsing functionality based on the WebKit engine.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a id="head_Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *WebKitBrowser*) |
| classname | string | mandatory | Class name: *WebKitBrowser* |
| locator | string | mandatory | Library name: *libThunderWebKitBrowser.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |
| configuration | object | optional | *...* |
| configuration?.useragent | string | optional | The UserAgent used during communication with the web server |
| configuration?.url | string | optional | The URL that is loaded upon starting the browser |
| configuration?.injectedbundle | string | optional | The name of the .so loaded to extend the HTML5 with customized JavaScript handlers |
| configuration?.cookiestorage | string | optional | Post-fixed path to the persistent location on where cookies will be stored |
| configuration?.localstorage | string | optional | Post-fixed path to the persistent location on where local-storage will be stored |
| configuration?.certificatecheck | boolean | optional | If enabled, SSL certificate error will lead to a page error |
| configuration?.javascript | object | optional | *...* |
| configuration?.javascript?.useLLInt | boolean | optional | Enable Low Level Interpreter |
| configuration?.javascript?.useJIT | boolean | optional | Enable JIT |
| configuration?.javascript?.useDFG | boolean | optional | Enable Data-Flow-Graph-JIT compiler |
| configuration?.javascript?.useFTL | boolean | optional | Enable Faster-Than-Light-JIT compiler |
| configuration?.javascript?.useDOM | boolean | optional | Enable the DOM-JIT compiler |
| configuration?.javascript?.dumpOptions | string | optional | Dump options: 0 = None, 1 = Overridden only, 2 = All, 3 = Verbose |
| configuration?.secure | boolean | optional | If false, every domain can access any other domain, otherwise fill in fields in whitelist definition table |
| configuration?.whitelist | object | optional | *...* |
| configuration?.whitelist?.origin | string | optional | Origin domain allowed to access domains in domain |
| configuration?.whitelist?.domain | array | optional | *...* |
| configuration?.whitelist?.domain[#] | string | mandatory | Domain allowed to access from origin |
| configuration?.whitelist?.subdomain | string | optional | Whether it is also OK to access subdomains of domains listed in domain |
| configuration?.localstorageenabled | boolean | optional | Controls the local storage availability |
| configuration?.logtosystemconsoleenabled | boolean | optional | Enable page logging to system console (stderr) |
| configuration?.watchdogchecktimeoutinseconds | integer | optional | How often to check main event loop for responsiveness (0 - disable) |
| configuration?.watchdoghangthresholdtinseconds | integer | optional | The amount of time to give a process to recover before declaring a hang state |
| configuration?.loadblankpageonsuspendenabled | boolean | optional | Load 'about:blank' before suspending the page |

<a id="head_Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- IWebBrowser ([IBrowser.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IBrowser.h)) (version 1.0.0) (uncompliant-extended format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- IWebBrowserExt ([IBrowser.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IBrowser.h)) (version 1.0.0) (uncompliant-extended format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- IBrowserResources ([IBrowser.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IBrowser.h)) (version 1.0.0) (uncompliant-extended format)
- IBrowserSecurity ([IBrowser.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IBrowser.h)) (version 1.0.0) (uncompliant-extended format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- IBrowserScripting ([IBrowser.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IBrowser.h)) (version 1.0.0) (uncompliant-extended format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

- IBrowserCookieJar ([IBrowser.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IBrowser.h)) (version 1.0.0) (uncompliant-extended format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a id="head_Methods"></a>
# Methods

The following methods are provided by the WebKitBrowser plugin:

Built-in methods:

| Method | Description |
| :-------- | :-------- |
| [versions](#method_versions) | Retrieves a list of JSON-RPC interfaces offered by this service |
| [exists](#method_exists) | Checks if a JSON-RPC method or property exists |
| [register](#method_register) | Registers for an asynchronous JSON-RPC notification |
| [unregister](#method_unregister) | Unregisters from an asynchronous JSON-RPC notification |

WebBrowser interface methods:

| Method | Description |
| :-------- | :-------- |
| [collectgarbage](#method_collectgarbage) | Initiate garbage collection |

WebBrowserExt interface methods:

| Method | Description |
| :-------- | :-------- |
| [delete](#method_delete) / [deletedir](#method_delete) | Removes contents of a directory from the persistent storage |

BrowserScripting interface methods:

| Method | Description |
| :-------- | :-------- |
| [runjavascript](#method_runjavascript) | Run javascript in main frame |
| [adduserscript](#method_adduserscript) | Add user script to be executed at document start |
| [removealluserscripts](#method_removealluserscripts) | Remove all user scripts |

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
  "method": "WebKitBrowser.1.versions"
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

This method will return *True* for the following methods/properties: *url, visibility, fps, headers, useragent, localstorageenabled, httpcookieacceptpolicy, bridgereply, bridgeevent, languages, userScripts, userStyleSheets, securityprofile, mixedcontentpolicy, cookiejar, versions, exists, register, unregister, collectgarbage, deletedir, runjavascript, adduserscript, removealluserscripts*.

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
  "method": "WebKitBrowser.1.exists",
  "params": {
    "method": "url"
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

This method supports the following event names: *[loadfinished](#notification_loadfinished), [loadfailed](#notification_loadfailed), [urlchange](#notification_urlchange), [visibilitychange](#notification_visibilitychange), [pageclosure](#notification_pageclosure), [bridgequery](#notification_bridgequery), [cookiejarchanged](#notification_cookiejarchanged)*.

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
  "method": "WebKitBrowser.1.register",
  "params": {
    "event": "loadfinished",
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

This method supports the following event names: *[loadfinished](#notification_loadfinished), [loadfailed](#notification_loadfailed), [urlchange](#notification_urlchange), [visibilitychange](#notification_visibilitychange), [pageclosure](#notification_pageclosure), [bridgequery](#notification_bridgequery), [cookiejarchanged](#notification_cookiejarchanged)*.

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
  "method": "WebKitBrowser.1.unregister",
  "params": {
    "event": "loadfinished",
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

<a id="method_collectgarbage"></a>
## *collectgarbage [<sup>method</sup>](#head_Methods)*

Initiate garbage collection.

### Parameters

This method takes no parameters.

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
  "method": "WebKitBrowser.1.collectgarbage"
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

<a id="method_delete"></a>
## *delete [<sup>method</sup>](#head_Methods)*

Removes contents of a directory from the persistent storage.

> ``deletedir`` is an alternative name for this method. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.path | string | mandatory | Path to directory (within the persistent storage) to delete contents of |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | null | mandatory | Always null |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_UNKNOWN_KEY``` | The given path cannot be empty |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.delete",
  "params": {
    "path": ".cache/wpe/disk-cache"
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

<a id="method_runjavascript"></a>
## *runjavascript [<sup>method</sup>](#head_Methods)*

Run javascript in main frame.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.script | string | mandatory | Utf8 encoded JS code string |

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
  "method": "WebKitBrowser.1.runjavascript",
  "params": {
    "script": "..."
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

<a id="method_adduserscript"></a>
## *adduserscript [<sup>method</sup>](#head_Methods)*

Add user script to be executed at document start.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.script | string | mandatory | Utf8 encoded JS code string |
| params.topframeonly | boolean | mandatory | Enable only top frame |

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
  "method": "WebKitBrowser.1.adduserscript",
  "params": {
    "script": "...",
    "topframeonly": false
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

<a id="method_removealluserscripts"></a>
## *removealluserscripts [<sup>method</sup>](#head_Methods)*

Remove all user scripts.

### Parameters

This method takes no parameters.

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
  "method": "WebKitBrowser.1.removealluserscripts"
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

The following properties are provided by the WebKitBrowser plugin:

WebBrowser interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [url](#property_url) | read/write | Page loaded in the browser |
| [visibility](#property_visibility) | read/write | Browser window visibility state |
| [fps](#property_fps) | read-only | Current framerate the browser is rendering at |
| [headers](#property_headers) | read/write | Headers to send on all requests that the browser makes |
| [useragent](#property_useragent) | read/write | UserAgent string used by the browser |
| [localstorageenabled](#property_localstorageenabled) | read/write | Controls the local storage availability |
| [httpcookieacceptpolicy](#property_httpcookieacceptpolicy) | read/write | HTTP cookies accept policy |
| [bridgereply](#property_bridgereply) | write-only | Response for legacy $badger |
| [bridgeevent](#property_bridgeevent) | write-only | Send legacy $badger event |

WebBrowserExt interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [languages](#property_languages) | read/write | User preferred languages |

BrowserResources interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [userScripts](#property_userScripts) | read/write | User scripts used by the browser |
| [userStyleSheets](#property_userStyleSheets) | read/write | User style sheets used by the browser |

BrowserSecurity interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [securityprofile](#property_securityprofile) | read/write | Security profile for secure connections |
| [mixedcontentpolicy](#property_mixedcontentpolicy) | read/write | Mixed content policy |

BrowserCookieJar interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [cookiejar](#property_cookiejar) | read/write | Get/Set CookieJar config details |

<a id="property_url"></a>
## *url [<sup>property</sup>](#head_Properties)*

Provides access to the page loaded in the browser.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Page loaded in the browser |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Loaded URL |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.url"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "https://example.com"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.url",
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

<a id="property_visibility"></a>
## *visibility [<sup>property</sup>](#head_Properties)*

Provides access to the browser window visibility state.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Browser window visibility state (must be one of the following: *hidden, visible*) |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Visiblity state (must be one of the following: *hidden, visible*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.visibility"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "visible"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.visibility",
  "params": "visible"
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

<a id="property_fps"></a>
## *fps [<sup>property</sup>](#head_Properties)*

Provides access to the current framerate the browser is rendering at.

> This property is **read-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | integer | mandatory | Current FPS |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.fps"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": 0
}
```

<a id="property_headers"></a>
## *headers [<sup>property</sup>](#head_Properties)*

Provides access to the headers to send on all requests that the browser makes.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | opaque object | mandatory | Headers to send on all requests that the browser makes |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | opaque object | mandatory | Single string containing a list of headers |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.headers"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {}
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.headers",
  "params": {}
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

<a id="property_useragent"></a>
## *useragent [<sup>property</sup>](#head_Properties)*

Provides access to the userAgent string used by the browser.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | UserAgent string used by the browser |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | UserAgent value |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.useragent"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "Mozilla/5.0 (Linux; x86_64 GNU/Linux) AppleWebKit/601.1 (KHTML, like Gecko) Version/8.0 Safari/601.1 WP"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.useragent",
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

<a id="property_localstorageenabled"></a>
## *localstorageenabled [<sup>property</sup>](#head_Properties)*

Provides access to the controls the local storage availability.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | boolean | mandatory | Controls the local storage availability |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | boolean | mandatory | Controls the local storage availability |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.localstorageenabled"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": false
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.localstorageenabled",
  "params": false
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

<a id="property_httpcookieacceptpolicy"></a>
## *httpcookieacceptpolicy [<sup>property</sup>](#head_Properties)*

Provides access to the HTTP cookies accept policy.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | HTTP cookies accept policy (must be one of the following: *always, exclusivelyfrommaindocumentdomain, never, onlyfrommaindocumentdomain*) |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | HTTP Cookie Accept Policy Type (must be one of the following: *always, exclusivelyfrommaindocumentdomain, never, onlyfrommaindocumentdomain*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.httpcookieacceptpolicy"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "always"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.httpcookieacceptpolicy",
  "params": "never"
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

<a id="property_bridgereply"></a>
## *bridgereply [<sup>property</sup>](#head_Properties)*

Provides access to the response for legacy $badger.

> This property is **write-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Base64 encoded JSON string response to be delivered to $badger.callback |

### Example

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.bridgereply",
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

<a id="property_bridgeevent"></a>
## *bridgeevent [<sup>property</sup>](#head_Properties)*

Provides access to the send legacy $badger event.

> This property is **write-only**.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Base64 encoded JSON string response to be delivered to window.$badger.event |

### Example

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.bridgeevent",
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

<a id="property_languages"></a>
## *languages [<sup>property</sup>](#head_Properties)*

Provides access to the user preferred languages.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | User preferred languages |
| (property)[#] | string | mandatory | *...* |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | List of langauges preferred by the user |
| (property)[#] | string | mandatory | *...* |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.languages"
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

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.languages",
  "params": [
    "..."
  ]
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

<a id="property_userScripts"></a>
## *userScripts [<sup>property</sup>](#head_Properties)*

Provides access to the user scripts used by the browser.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | User scripts used by the browser |
| (property)[#] | string | mandatory | *...* |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | JSON array containing URIs pointing to user scripts, supported protocols: file:// |
| (property)[#] | string | mandatory | *...* |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.userScripts"
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

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.userScripts",
  "params": [
    "..."
  ]
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

<a id="property_userStyleSheets"></a>
## *userStyleSheets [<sup>property</sup>](#head_Properties)*

Provides access to the user style sheets used by the browser.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | User style sheets used by the browser |
| (property)[#] | string | mandatory | *...* |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | JSON array containing URIs pointing to user style sheets, supported protocols: file:// |
| (property)[#] | string | mandatory | *...* |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.userStyleSheets"
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

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.userStyleSheets",
  "params": [
    "..."
  ]
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

<a id="property_securityprofile"></a>
## *securityprofile [<sup>property</sup>](#head_Properties)*

Provides access to the security profile for secure connections.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Security profile for secure connections |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Security profile for secure connections |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.securityprofile"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "compatible"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.securityprofile",
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

<a id="property_mixedcontentpolicy"></a>
## *mixedcontentpolicy [<sup>property</sup>](#head_Properties)*

Provides access to the mixed content policy.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Mixed content policy (must be one of the following: *allowed, blocked*) |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Mixed content policy type (must be one of the following: *allowed, blocked*) |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.mixedcontentpolicy"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "blocked"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.mixedcontentpolicy",
  "params": "blocked"
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

<a id="property_cookiejar"></a>
## *cookiejar [<sup>property</sup>](#head_Properties)*

Provides access to the get/Set CookieJar config details.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | object | mandatory | Get/Set CookieJar config details |
| (property).version | integer | mandatory | Version of payload format |
| (property).checksum | integer | mandatory | The checksum of the string used for payload creation |
| (property).payload | string | mandatory | Base64 string representation of compressed and encrypted cookies |

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | object | mandatory | Config info CookieJar |
| (property).version | integer | mandatory | Version of payload format |
| (property).checksum | integer | mandatory | The checksum of the string used for payload creation |
| (property).payload | string | mandatory | Base64 string representation of compressed and encrypted cookies |

### Example

#### Get Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.cookiejar"
}
```

#### Get Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "version": 0,
    "checksum": 0,
    "payload": "..."
  }
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.cookiejar",
  "params": {
    "version": 0,
    "checksum": 0,
    "payload": "..."
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

<a id="head_Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the WebKitBrowser plugin:

WebBrowser interface events:

| Notification | Description |
| :-------- | :-------- |
| [loadfinished](#notification_loadfinished) | Initial HTML document has been completely loaded and parsed |
| [loadfailed](#notification_loadfailed) | Browser failed to load page |
| [urlchange](#notification_urlchange) | Signals a URL change in the browser |
| [visibilitychange](#notification_visibilitychange) | Signals a visibility change of the browser |
| [pageclosure](#notification_pageclosure) | Notifies that the web page requests to close its window |
| [bridgequery](#notification_bridgequery) | A Base64 encoded JSON message from legacy $badger bridge |

BrowserCookieJar interface events:

| Notification | Description |
| :-------- | :-------- |
| [cookiejarchanged](#notification_cookiejarchanged) | Notifies that cookies were added, removed or modified |

<a id="notification_loadfinished"></a>
## *loadfinished [<sup>notification</sup>](#head_Notifications)*

Initial HTML document has been completely loaded and parsed.

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.url | string | mandatory | The URL that has been loaded |
| params.httpstatus | integer | mandatory | The response code of main resource request |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.register",
  "params": {
    "event": "loadfinished",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.loadfinished",
  "params": {
    "url": "https://example.com",
    "httpstatus": 200
  }
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.loadfinished``.

<a id="notification_loadfailed"></a>
## *loadfailed [<sup>notification</sup>](#head_Notifications)*

Browser failed to load page.

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.url | string | mandatory | The URL that has been failed to load |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.register",
  "params": {
    "event": "loadfailed",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.loadfailed",
  "params": {
    "url": "https://example.com"
  }
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.loadfailed``.

<a id="notification_urlchange"></a>
## *urlchange [<sup>notification</sup>](#head_Notifications)*

Signals a URL change in the browser.

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.url | string | mandatory | The URL that has been loaded or requested |
| params.loaded | boolean | mandatory | Loaded (true) or not (false) |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.register",
  "params": {
    "event": "urlchange",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.urlchange",
  "params": {
    "url": "https://example.com",
    "loaded": false
  }
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.urlchange``.

<a id="notification_visibilitychange"></a>
## *visibilitychange [<sup>notification</sup>](#head_Notifications)*

Signals a visibility change of the browser.

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.hidden | boolean | mandatory | Hidden (true) or Visible (false) |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.register",
  "params": {
    "event": "visibilitychange",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.visibilitychange",
  "params": {
    "hidden": false
  }
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.visibilitychange``.

<a id="notification_pageclosure"></a>
## *pageclosure [<sup>notification</sup>](#head_Notifications)*

Notifies that the web page requests to close its window.

### Notification Parameters

This notification carries no parameters.

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.register",
  "params": {
    "event": "pageclosure",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.pageclosure"
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.pageclosure``.

<a id="notification_bridgequery"></a>
## *bridgequery [<sup>notification</sup>](#head_Notifications)*

A Base64 encoded JSON message from legacy $badger bridge.

### Notification Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.message | string | mandatory | Requested action |

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.register",
  "params": {
    "event": "bridgequery",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.bridgequery",
  "params": {
    "message": "..."
  }
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.bridgequery``.

<a id="notification_cookiejarchanged"></a>
## *cookiejarchanged [<sup>notification</sup>](#head_Notifications)*

Notifies that cookies were added, removed or modified.

### Notification Parameters

This notification carries no parameters.

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.register",
  "params": {
    "event": "cookiejarchanged",
    "id": "myid"
  }
}
```

#### Notification

```json
{
  "jsonrpc": "2.0",
  "method": "myid.cookiejarchanged"
}
```

> The *client ID* parameter is passed within the notification designator, i.e. ``<client-id>.cookiejarchanged``.

