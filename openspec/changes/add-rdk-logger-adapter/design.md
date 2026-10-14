## Adapter boundary

The adapter implements the four-function C contract supplied by Thunder Messaging:

- initialize and deinitialize the backend,
- query whether a module and mapped level are enabled,
- submit a fully formed payload synchronously.

Thunder owns routing and passes the existing module name, message type, category, and payload. The adapter owns RDK Logger naming, level mapping, payload decoration, and backend error handling.

## Enablement and lifecycle

The adapter initializes once for each process that opens Messaging and releases its backend state during teardown. Messaging may query the adapter during startup and when new modules are announced. Repeated module/level queries are cached by the Messaging integration.

If the backend is unavailable or rejects initialization or an enablement query, the adapter reports failure without changing Thunder's handler or DirectOutput behavior.

## Build variants

The production implementation links against the target RDK Logger SDK. The mock implementation uses the same C contract, writes submissions to the console, and supports SDK-independent builds and focused tests.

## Mapping ownership

The RDK Logger-specific category, level, payload, and platform-registration rules are documented in `mapping.md` and owned by this adapter. Thunder remains unaware of those backend details.