# RDK Logger Mapping for Thunder External Direct Output

## Purpose

This document describes how the RDK Logger adapter maps Thunder Messaging events when the optional External Direct route is enabled. It is intended for adapter implementation, platform configuration, operational review, and cross-team agreement. External Direct itself is not specific to this backend.

## Category Name

The adapter sends every message under this RDK Logger category format:

```
LOG.RDK.THUNDER.<MODULE_NAME>
```

`MODULE_NAME` is Thunder's existing module identifier. The adapter does not alter it: it does not normalize, truncate, validate, or resolve collisions.

Examples:

| Thunder module | RDK Logger category |
| --- | --- |
| `Thunder` | `LOG.RDK.THUNDER.Thunder` |
| `WebKitBrowser` | `LOG.RDK.THUNDER.WebKitBrowser` |
| `DeviceInfo` | `LOG.RDK.THUNDER.DeviceInfo` |
| `SysLog` | `LOG.RDK.THUNDER.SysLog` |

## Level Mapping

| Thunder message | RDK Logger level |
| --- | --- |
| Assertion or fatal-like event | `FATAL` |
| Warning report | `WARN` |
| Trace: Fatal or Crash | `FATAL` |
| Trace: Error | `ERROR` |
| Trace: Warning or Warn | `WARN` |
| Trace: Notice | `NOTICE` |
| Other trace | `TRACE` |
| Telemetry | `NOTICE` |
| Operational stream | `TRACE` |
| Log: Crash or Fatal | `FATAL` |
| Log: Error | `ERROR` |
| Log: Warning or Warn | `WARN` |
| Log: Notice | `NOTICE` |
| Other log | `INFO` |

## Payload Mapping

The adapter sends the message payload unchanged for explicitly classified levels, such as `ERROR`, `WARN`, `NOTICE`, and `FATAL`.

For default-level logging and tracing, the adapter adds the non-empty Thunder category to retain useful context:

```
<CATEGORY>: <PAYLOAD>
```

Examples:

| Thunder input | RDK Logger payload |
| --- | --- |
| Logging category `Information`, payload `Connected` | `Information: Connected` |
| Trace category `Timing`, payload `Frame rendered` | `Timing: Frame rendered` |
| Error logging payload `Connection failed` | `Connection failed` |

Thunder metadata is otherwise used only to select the RDK Logger category and level.

## Delivery Rules

- RDK Logger is an independent output destination; enabling it does not replace MessageControl or Thunder DirectOutput.
- RDK Logger enablement is evaluated per Thunder module and mapped level.
- Telemetry is not sent to RDK Logger when it is also routed to the Thunder handler, because MessageControl forwards that handler route to T2.
- Telemetry routed externally without the handler uses the `NOTICE` mapping above.
- If RDK Logger is unavailable or rejects a module/level, existing Thunder local routes continue unchanged.

## Ownership

The RDK Logger adapter owns this mapping and calls the RDK Logger API. The RDK Logger/platform team owns registration and enablement of `LOG.RDK.THUNDER.<MODULE_NAME>` categories in Log4C or the target-equivalent configuration. Thunder does not generate, install, or maintain that platform configuration.