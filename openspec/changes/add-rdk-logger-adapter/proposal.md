## Why

Thunder Messaging provides a generic External Direct adapter contract, but the RDK Logger implementation and its platform-specific mapping belong in ThunderNanoServicesRDK. Keeping the implementation here lets RDK deployments evolve their logger integration without adding RDK Logger dependencies to Thunder.

## What Changes

- Build the RDK Logger implementation as the standalone `ThunderMessagingExternalOutput` shared library.
- Implement the generic External Direct C contract exposed by Thunder Messaging.
- Map Thunder modules, message types, categories, levels, and payloads to RDK Logger conventions.
- Keep a mock implementation available for SDK-independent development and tests.
- Document the RDK Logger category and level mapping in this change.

## Goals

- Keep backend-specific behavior in the adapter repository.
- Preserve the generic Thunder Messaging integration boundary.
- Allow the adapter to evaluate enablement independently and submit messages synchronously.
- Make the adapter buildable with and without the target RDK Logger SDK.

## Non-Goals

- Changing Thunder Messaging routing or its public adapter contract.
- Replacing MessageControl or Thunder DirectOutput.
- Defining or installing target platform logger configuration.
- Adding asynchronous buffering or a new logger administration API.