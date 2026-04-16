/*
 * Mock version of the t2 telemetry API header.
 *
 * Provides the same function declarations as the real
 * telemetry_busmessage_sender.h so that TelemetryBackendT2
 * can be compiled against the mock library without any
 * source-level differences.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void t2_init(char* component);
int  t2_event_s(const char* marker, const char* value);
int  t2_event_d(const char* marker, int value);
int  t2_event_f(const char* marker, double value);
void t2_uninit(void);

#ifdef __cplusplus
}
#endif
