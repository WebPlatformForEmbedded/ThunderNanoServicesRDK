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
- IWebBrowserExt ([IBrowser.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IBrowser.h)) (version 1.0.0) (uncompliant-extended format)
- IBrowserResources ([IBrowser.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IBrowser.h)) (version 1.0.0) (uncompliant-extended format)
- IBrowserSecurity ([IBrowser.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IBrowser.h)) (version 1.0.0) (uncompliant-extended format)
- IBrowserScripting ([IBrowser.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IBrowser.h)) (version 1.0.0) (uncompliant-extended format)
- IBrowserCookieJar ([IBrowser.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IBrowser.h)) (version 1.0.0) (uncompliant-extended format)

<a name="head.Methods"></a>
# Methods

The following methods are provided by the WebKitBrowser plugin:

WebBrowser interface methods:

| Method | Description |
| :-------- | :-------- |
| [collectGarbage](#method.collectGarbage) | Initiate garbage collection |

WebBrowserExt interface methods:

| Method | Description |
| :-------- | :-------- |
| [deletedir](#method.deletedir) / [delete](#method.deletedir) | Removes contents of a directory from the persistent storage |

BrowserScripting interface methods:

| Method | Description |
| :-------- | :-------- |
| [runJavaScript](#method.runJavaScript) | Run javascript in main frame |
| [addUserScript](#method.addUserScript) | Add user script to be executed at document start |
| [removeAllUserScripts](#method.removeAllUserScripts) | Remove all user scripts |

<a name="method.collectGarbage"></a>
## *collectGarbage [<sup>method</sup>](#head.Methods)*

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
  "method": "WebKitBrowser.1.collectGarbage"
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

<a name="method.deletedir"></a>
## *deletedir [<sup>method</sup>](#head.Methods)*

Removes contents of a directory from the persistent storage.

> ``delete`` is an alternative name for this method. This name is **deprecated** and may be removed in the future. It is not recommended for use in new implementations.

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
  "method": "WebKitBrowser.1.deletedir",
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

<a name="method.runJavaScript"></a>
## *runJavaScript [<sup>method</sup>](#head.Methods)*

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
  "method": "WebKitBrowser.1.runJavaScript",
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

<a name="method.addUserScript"></a>
## *addUserScript [<sup>method</sup>](#head.Methods)*

Add user script to be executed at document start.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.script | string | mandatory | Utf8 encoded JS code string |
| params.topFrameOnly | boolean | mandatory | Enable only top frame |

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
  "method": "WebKitBrowser.1.addUserScript",
  "params": {
    "script": "...",
    "topFrameOnly": false
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

<a name="method.removeAllUserScripts"></a>
## *removeAllUserScripts [<sup>method</sup>](#head.Methods)*

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
  "method": "WebKitBrowser.1.removeAllUserScripts"
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
| [userAgent](#property.userAgent) | read/write | UserAgent string used by the browser |
| [localStorageEnabled](#property.localStorageEnabled) | read/write | Controls the local storage availability |
| [httpCookieAcceptPolicy](#property.httpCookieAcceptPolicy) | read/write | HTTP cookies accept policy |
| [bridgeReply](#property.bridgeReply) | write-only | Response for legacy $badger |
| [bridgeEvent](#property.bridgeEvent) | write-only | Send legacy $badger event |

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
| [securityProfile](#property.securityProfile) | read/write | Security profile for secure connections |
| [mixedContentPolicy](#property.mixedContentPolicy) | read/write | Mixed content policy |

BrowserCookieJar interface properties:

| Property | R/W | Description |
| :-------- | :-------- | :-------- |
| [cookieJar](#property.cookieJar) | read/write | Get/Set CookieJar config details |

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

<a name="property.userAgent"></a>
## *userAgent [<sup>property</sup>](#head.Properties)*

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
  "method": "WebKitBrowser.1.userAgent"
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
  "method": "WebKitBrowser.1.userAgent",
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

<a name="property.localStorageEnabled"></a>
## *localStorageEnabled [<sup>property</sup>](#head.Properties)*

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
  "method": "WebKitBrowser.1.localStorageEnabled"
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
  "method": "WebKitBrowser.1.localStorageEnabled",
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

<a name="property.httpCookieAcceptPolicy"></a>
## *httpCookieAcceptPolicy [<sup>property</sup>](#head.Properties)*

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
  "method": "WebKitBrowser.1.httpCookieAcceptPolicy"
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
  "method": "WebKitBrowser.1.httpCookieAcceptPolicy",
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

<a name="property.bridgeReply"></a>
## *bridgeReply [<sup>property</sup>](#head.Properties)*

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
  "method": "WebKitBrowser.1.bridgeReply",
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

<a name="property.bridgeEvent"></a>
## *bridgeEvent [<sup>property</sup>](#head.Properties)*

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
  "method": "WebKitBrowser.1.bridgeEvent",
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

<a name="property.securityProfile"></a>
## *securityProfile [<sup>property</sup>](#head.Properties)*

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
  "method": "WebKitBrowser.1.securityProfile"
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
  "method": "WebKitBrowser.1.securityProfile",
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

<a name="property.mixedContentPolicy"></a>
## *mixedContentPolicy [<sup>property</sup>](#head.Properties)*

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
  "method": "WebKitBrowser.1.mixedContentPolicy"
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
  "method": "WebKitBrowser.1.mixedContentPolicy",
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

<a name="property.cookieJar"></a>
## *cookieJar [<sup>property</sup>](#head.Properties)*

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
  "method": "WebKitBrowser.1.cookieJar"
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
  "method": "WebKitBrowser.1.cookieJar",
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
| [loadFinished](#notification.loadFinished) | Initial HTML document has been completely loaded and parsed |
| [loadFailed](#notification.loadFailed) | Browser failed to load page |
| [urlChange](#notification.urlChange) | Signals a URL change in the browser |
| [visibilityChange](#notification.visibilityChange) | Signals a visibility change of the browser |
| [pageClosure](#notification.pageClosure) | Notifies that the web page requests to close its window |
| [bridgeQuery](#notification.bridgeQuery) | A Base64 encoded JSON message from legacy $badger bridge |

BrowserCookieJar interface events:

| Notification | Description |
| :-------- | :-------- |
| [cookieJarChanged](#notification.cookieJarChanged) | Notifies that cookies were added, removed or modified |

<a name="notification.loadFinished"></a>
## *loadFinished [<sup>notification</sup>](#head.Notifications)*

Initial HTML document has been completely loaded and parsed.

### Parameters

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
    "event": "loadFinished",
    "id": "client"
  }
}
```

#### Message

```json
{
  "jsonrpc": "2.0",
  "method": "client.loadFinished",
  "params": {
    "url": "https://example.com",
    "httpstatus": 200
  }
}
```

<a name="notification.loadFailed"></a>
## *loadFailed [<sup>notification</sup>](#head.Notifications)*

Browser failed to load page.

### Parameters

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
    "event": "loadFailed",
    "id": "client"
  }
}
```

#### Message

```json
{
  "jsonrpc": "2.0",
  "method": "client.loadFailed",
  "params": {
    "url": "https://example.com"
  }
}
```

<a name="notification.urlChange"></a>
## *urlChange [<sup>notification</sup>](#head.Notifications)*

Signals a URL change in the browser.

### Parameters

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
    "event": "urlChange",
    "id": "client"
  }
}
```

#### Message

```json
{
  "jsonrpc": "2.0",
  "method": "client.urlChange",
  "params": {
    "url": "https://example.com",
    "loaded": false
  }
}
```

<a name="notification.visibilityChange"></a>
## *visibilityChange [<sup>notification</sup>](#head.Notifications)*

Signals a visibility change of the browser.

### Parameters

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
    "event": "visibilityChange",
    "id": "client"
  }
}
```

#### Message

```json
{
  "jsonrpc": "2.0",
  "method": "client.visibilityChange",
  "params": {
    "hidden": false
  }
}
```

<a name="notification.pageClosure"></a>
## *pageClosure [<sup>notification</sup>](#head.Notifications)*

Notifies that the web page requests to close its window.

### Parameters

This notification carries no parameters.

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.register",
  "params": {
    "event": "pageClosure",
    "id": "client"
  }
}
```

#### Message

```json
{
  "jsonrpc": "2.0",
  "method": "client.pageClosure"
}
```

<a name="notification.bridgeQuery"></a>
## *bridgeQuery [<sup>notification</sup>](#head.Notifications)*

A Base64 encoded JSON message from legacy $badger bridge.

### Parameters

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
    "event": "bridgeQuery",
    "id": "client"
  }
}
```

#### Message

```json
{
  "jsonrpc": "2.0",
  "method": "client.bridgeQuery",
  "params": {
    "message": "..."
  }
}
```

<a name="notification.cookieJarChanged"></a>
## *cookieJarChanged [<sup>notification</sup>](#head.Notifications)*

Notifies that cookies were added, removed or modified.

### Parameters

This notification carries no parameters.

### Example

#### Registration

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "WebKitBrowser.1.register",
  "params": {
    "event": "cookieJarChanged",
    "id": "client"
  }
}
```

#### Message

```json
{
  "jsonrpc": "2.0",
  "method": "client.cookieJarChanged"
}
```

