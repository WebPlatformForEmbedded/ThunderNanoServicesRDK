# ThunderNanoServicesRDK Release Notes R5.3

## introduction

This document describes the new features and changes introduced in ThunderNanoServicesRDK R5.3 (compared to the latest R5.2 release).

##  Changes and New Features

### Change: Multiple plugins changed

Multiple plugins were changed for Thunder 5.3 and the added @json tag in IDL header file go(as replacement for the previous meta data file for the JSON-RPC interface)
They were also adopted for the improved connection closed ASSERT and the handling of dangling proxies where applicable.

The Monitor plugin had several issues fixed as well as the MessageControl plugin.

### Feature: BridgeLink plugin

The BridgeLink plugin was added, it is a plugin that exposes Remote plugins (from a Remote Thunder instance) via a Composite plugin locally.



