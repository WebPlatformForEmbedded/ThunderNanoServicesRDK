<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.Packager_Plugin"></a>
# Packager Plugin

**Version: 1.0**

**Status: :black_circle::white_circle::white_circle:**

Packager plugin for Thunder framework.

### Table of Contents

- [Introduction](#head.Introduction)
- [Description](#head.Description)
- [Configuration](#head.Configuration)
- [Interfaces](#head.Interfaces)
- [Methods](#head.Methods)

<a name="head.Introduction"></a>
# Introduction

<a name="head.Scope"></a>
## Scope

This document describes purpose and functionality of the Packager plugin. It includes detailed specification about its configuration and methods provided.

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

The Packager plugin allows installation of OPKG, IPKG and DEB packages to the system from a remote repository.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| callsign | string | mandatory | Plugin instance name (default: *Packager*) |
| classname | string | mandatory | Class name: *Packager* |
| locator | string | mandatory | Library name: *libThunderPackager.so* |
| startmode | string | mandatory | Determines in which state the plugin should be moved to at startup of the framework |

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- IPackager ([IPackager.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IPackager.h)) (version 1.0.0) (compliant format)
> This interface uses legacy ```lowercase``` naming convention. With the next major release the naming convention will change to ```camelCase```.

<a name="head.Methods"></a>
# Methods

The following methods are provided by the Packager plugin:

Packager interface methods:

| Method | Description |
| :-------- | :-------- |
| [install](#method.install) | Install a package given by a name, an URL or a file path |
| [synchronizerepository](#method.synchronizerepository) / [::](#method.synchronizerepository) | Synchronize repository manifest with a repository |

<a name="method.install"></a>
## *install [<sup>method</sup>](#head.Methods)*

Install a package given by a name, an URL or a file path.

### Parameters

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| params | object | mandatory | *...* |
| params.name | string | mandatory | Name, URL or file path of the package to install |
| params.version | string | mandatory | Version of the package to install |
| params.arch | string | mandatory | Architecture of the package to install |

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | null | mandatory | Always null |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_INPROGRESS``` | Other installation/synchronization is already in progress |
| ```ERROR_GENERAL``` | Opkg package manager not initialized successfully |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Packager.1.install",
  "params": {
    "name": "thunder-plugin-netflix",
    "version": "1.0",
    "arch": "arm"
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

<a name="method.synchronizerepository"></a>
## *synchronizerepository [<sup>method</sup>](#head.Methods)*

Synchronize repository manifest with a repository.

> ``::`` is an alternative name for this method.

### Parameters

This method takes no parameters.

### Result

| Name | Type | M/O | Description |
| :-------- | :-------- | :-------- | :-------- |
| result | null | mandatory | Always null |

### Errors

| Message | Description |
| :-------- | :-------- |
| ```ERROR_INPROGRESS``` | Other installation/synchronization is already in progress |
| ```ERROR_GENERAL``` | Opkg package manager not initialized successfully |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "Packager.1.synchronizerepository"
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

