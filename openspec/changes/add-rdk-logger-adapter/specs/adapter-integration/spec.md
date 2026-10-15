## ADDED Requirements

### Requirement: Implement the Thunder External Direct contract

The adapter SHALL implement the External Direct C interface provided by Thunder Messaging without changing its function signatures or ownership rules.

#### Scenario: Production adapter submits an enabled message

- **WHEN** Thunder initializes the adapter, confirms a module and level are enabled, and submits a payload
- **THEN** the adapter forwards the payload synchronously to RDK Logger using the mapping in `mapping.md`

#### Scenario: Disabled module or level

- **WHEN** Thunder queries a module and mapped level that is disabled
- **THEN** the adapter reports that the external destination is not enabled and does not submit a message

### Requirement: Keep backend details isolated

The adapter SHALL keep RDK Logger headers, libraries, naming, and level conventions out of Thunder's generic Messaging contract.

#### Scenario: SDK-independent build

- **WHEN** the mock adapter is selected
- **THEN** the adapter builds without the RDK Logger SDK and accepts the same lifecycle, query, and submission calls

#### Scenario: Backend failure

- **WHEN** RDK Logger initialization or submission is unavailable
- **THEN** the adapter reports failure or ignores the failed external delivery without taking ownership of Thunder's local output routes