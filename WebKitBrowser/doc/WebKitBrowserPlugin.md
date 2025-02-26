<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.WebKit_Browser_Plugin"></a>
# WebKit Browser Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

WebKitBrowser plugin for Thunder framework.

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

This document describes purpose and functionality of the WebKitBrowser plugin. It includes detailed specification about its configuration, methods and properties as well as sent notifications.

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

The WebKitBrowser plugin provides web browsing functionality based on the WebKit engine.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
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
| configuration?.whitelist?.domain[#] | string | optional | Domain allowed to access from origin |
| configuration?.whitelist?.subdomain | string | optional | Whether it is also OK to access subdomains of domains listed in domain |
| configuration?.localstorageenabled | boolean | optional | Controls the local storage availability |
| configuration?.logtosystemconsoleenabled | boolean | optional | Enable page logging to system console (stderr) |
| configuration?.watchdogchecktimeoutinseconds | integer | optional | How often to check main event loop for responsiveness (0 - disable) |
| configuration?.watchdoghangthresholdtinseconds | integer | optional | The amount of time to give a process to recover before declaring a hang state |
| configuration?.loadblankpageonsuspendenabled | boolean | optional | Load 'about:blank' before suspending the page |

<a name="head.Interfaces"></a>
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

<a name="head.Methods"></a>
# Methods

The following methods are provided by the WebKitBrowser plugin:

WebBrowser interface methods:

| Method | Description |
| :-------- | :-------- |
| [collectgarbage](#method.collectgarbage) | Initiate garbage collection |

WebBrowserExt interface methods:

| Method | Description |
| :-------- | :-------- |
| [delete](#method.delete) / [deletedir](#method.delete) | Removes contents of a directory from the persistent storage |

BrowserScripting interface methods:

| Method | Description |
| :-------- | :-------- |
| [runjavascript](#method.runjavascript) | Run javascript in main frame |
| [adduserscript](#method.adduserscript) | Add user script to be executed at document start |
| [removealluserscripts](#method.removealluserscripts) | Remove all user scripts |

<a name="method.collectgarbage"></a>
## *collectgarbage [<sup>method</sup>](#head.Methods)*

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

<a name="method.delete"></a>
## *delete [<sup>method</sup>](#head.Methods)*

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

<a name="method.runjavascript"></a>
## *runjavascript [<sup>method</sup>](#head.Methods)*

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

<a name="method.adduserscript"></a>
## *adduserscript [<sup>method</sup>](#head.Methods)*

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

<a name="method.removealluserscripts"></a>
## *removealluserscripts [<sup>method</sup>](#head.Methods)*

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

<a name="head.Properties"></a>
# Properties

The following properties are provided by the WebKitBrowser plugin:

WebBrowser interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [url](#property.url) | read/write | Page loaded in the browser |
| [visibility](#property.visibility) | read/write | Browser window visibility state |
| [fps](#property.fps) | read-only | Current framerate the browser is rendering at |
| [headers](#property.headers) | read/write | Headers to send on all requests that the browser makes |
| [useragent](#property.useragent) | read/write | UserAgent string used by the browser |
| [localstorageenabled](#property.localstorageenabled) | read/write | Controls the local storage availability |
| [httpcookieacceptpolicy](#property.httpcookieacceptpolicy) | read/write | HTTP cookies accept policy |
| [bridgereply](#property.bridgereply) | write-only | Response for legacy $badger |
| [bridgeevent](#property.bridgeevent) | write-only | Send legacy $badger event |

WebBrowserExt interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [languages](#property.languages) | read/write | User preferred languages |

BrowserResources interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [userScripts](#property.userScripts) | read/write | User scripts used by the browser |
| [userStyleSheets](#property.userStyleSheets) | read/write | User style sheets used by the browser |

BrowserSecurity interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [securityprofile](#property.securityprofile) | read/write | Security profile for secure connections |
| [mixedcontentpolicy](#property.mixedcontentpolicy) | read/write | Mixed content policy |

BrowserCookieJar interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [cookiejar](#property.cookiejar) | read/write | Get/Set CookieJar config details |

<a name="property.url"></a>
## *url [<sup>property</sup>](#head.Properties)*

Provides access to the page loaded in the browser.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Page loaded in the browser |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Loaded URL |

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

<a name="property.visibility"></a>
## *visibility [<sup>property</sup>](#head.Properties)*

Provides access to the browser window visibility state.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Browser window visibility state (must be one of the following: *hidden, visible*) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Visiblity state (must be one of the following: *hidden, visible*) |

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
  "result": "hidden"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.visibility",
  "params": "hidden"
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

<a name="property.fps"></a>
## *fps [<sup>property</sup>](#head.Properties)*

Provides access to the current framerate the browser is rendering at.

> This property is **read-only**.

### Value

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | integer | mandatory | Current FPS |

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

<a name="property.headers"></a>
## *headers [<sup>property</sup>](#head.Properties)*

Provides access to the headers to send on all requests that the browser makes.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | opaque object | mandatory | Headers to send on all requests that the browser makes |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | opaque object | mandatory | Single string containing a list of headers |

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

<a name="property.useragent"></a>
## *useragent [<sup>property</sup>](#head.Properties)*

Provides access to the userAgent string used by the browser.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | UserAgent string used by the browser |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | UserAgent value |

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

<a name="property.localstorageenabled"></a>
## *localstorageenabled [<sup>property</sup>](#head.Properties)*

Provides access to the controls the local storage availability.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | boolean | mandatory | Controls the local storage availability |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | boolean | mandatory | Controls the local storage availability |

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

<a name="property.httpcookieacceptpolicy"></a>
## *httpcookieacceptpolicy [<sup>property</sup>](#head.Properties)*

Provides access to the HTTP cookies accept policy.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | HTTP cookies accept policy (must be one of the following: *always, exclusivelyfrommaindocumentdomain, never, onlyfrommaindocumentdomain*) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | HTTP Cookie Accept Policy Type (must be one of the following: *always, exclusivelyfrommaindocumentdomain, never, onlyfrommaindocumentdomain*) |

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
  "params": "always"
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

<a name="property.bridgereply"></a>
## *bridgereply [<sup>property</sup>](#head.Properties)*

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

<a name="property.bridgeevent"></a>
## *bridgeevent [<sup>property</sup>](#head.Properties)*

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

<a name="property.languages"></a>
## *languages [<sup>property</sup>](#head.Properties)*

Provides access to the user preferred languages.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | User preferred languages |
| (property)[#] | string | mandatory | *...* |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | List of langauges preferred by the user |
| result[#] | string | mandatory | *...* |

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

<a name="property.userScripts"></a>
## *userScripts [<sup>property</sup>](#head.Properties)*

Provides access to the user scripts used by the browser.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | User scripts used by the browser |
| (property)[#] | string | mandatory | *...* |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | JSON array containing URIs pointing to user scripts, supported protocols: file:// |
| result[#] | string | mandatory | *...* |

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

<a name="property.userStyleSheets"></a>
## *userStyleSheets [<sup>property</sup>](#head.Properties)*

Provides access to the user style sheets used by the browser.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | array | mandatory | User style sheets used by the browser |
| (property)[#] | string | mandatory | *...* |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | array | mandatory | JSON array containing URIs pointing to user style sheets, supported protocols: file:// |
| result[#] | string | mandatory | *...* |

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

<a name="property.securityprofile"></a>
## *securityprofile [<sup>property</sup>](#head.Properties)*

Provides access to the security profile for secure connections.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Security profile for secure connections |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Security profile for secure connections |

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

<a name="property.mixedcontentpolicy"></a>
## *mixedcontentpolicy [<sup>property</sup>](#head.Properties)*

Provides access to the mixed content policy.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | string | mandatory | Mixed content policy (must be one of the following: *allowed, blocked*) |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | string | mandatory | Mixed content policy type (must be one of the following: *allowed, blocked*) |

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
  "result": "allowed"
}
```

#### Set Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.mixedcontentpolicy",
  "params": "allowed"
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

<a name="property.cookiejar"></a>
## *cookiejar [<sup>property</sup>](#head.Properties)*

Provides access to the get/Set CookieJar config details.

### Value

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| (property) | object | mandatory | Get/Set CookieJar config details |
| (property).version | integer | mandatory | Version of payload format |
| (property).checksum | integer | mandatory | The checksum of the string used for payload creation |
| (property).payload | string | mandatory | Base64 string representation of compressed and encrypted cookies |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | object | mandatory | Config info CookieJar |
| result.version | integer | mandatory | Version of payload format |
| result.checksum | integer | mandatory | The checksum of the string used for payload creation |
| result.payload | string | mandatory | Base64 string representation of compressed and encrypted cookies |

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

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the WebKitBrowser plugin:

WebBrowser interface events:

| Notification | Description |
| :-------- | :-------- |
| [loadfinished](#notification.loadfinished) | Initial HTML document has been completely loaded and parsed |
| [loadfailed](#notification.loadfailed) | Browser failed to load page |
| [urlchange](#notification.urlchange) | Signals a URL change in the browser |
| [visibilitychange](#notification.visibilitychange) | Signals a visibility change of the browser |
| [pageclosure](#notification.pageclosure) | Notifies that the web page requests to close its window |
| [bridgequery](#notification.bridgequery) | A Base64 encoded JSON message from legacy $badger bridge |

BrowserCookieJar interface events:

| Notification | Description |
| :-------- | :-------- |
| [cookiejarchanged](#notification.cookiejarchanged) | Notifies that cookies were added, removed or modified |

<a name="notification.loadfinished"></a>
## *loadfinished [<sup>notification</sup>](#head.Notifications)*

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

<a name="notification.loadfailed"></a>
## *loadfailed [<sup>notification</sup>](#head.Notifications)*

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

<a name="notification.urlchange"></a>
## *urlchange [<sup>notification</sup>](#head.Notifications)*

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

<a name="notification.visibilitychange"></a>
## *visibilitychange [<sup>notification</sup>](#head.Notifications)*

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

<a name="notification.pageclosure"></a>
## *pageclosure [<sup>notification</sup>](#head.Notifications)*

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

<a name="notification.bridgequery"></a>
## *bridgequery [<sup>notification</sup>](#head.Notifications)*

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

<a name="notification.cookiejarchanged"></a>
## *cookiejarchanged [<sup>notification</sup>](#head.Notifications)*

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

