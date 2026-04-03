/*
 * Mock implementation of the t2 telemetry API.
 *
 * Prints all events to stdout so they can be observed during testing.
 * Build as a shared library and make it findable via a CMake Find module.
 */

#include <stdio.h>
#include <inttypes.h>

void t2_init(char* component)
{
    printf("[t2-mock] t2_init(\"%s\")\n", component ? component : "(null)");
}

int t2_event_s(const char* marker, const char* value)
{
    printf("[t2-mock] t2_event_s(\"%s\", \"%s\")\n",
           marker ? marker : "(null)",
           value  ? value  : "(null)");

    return (0);
}

int t2_event_d(const char* marker, int value)
{
    printf("[t2-mock] t2_event_d(\"%s\", %d)\n",
           marker ? marker : "(null)",
           value);

    return (0);
}

int t2_event_f(const char* marker, double value)
{
    printf("[t2-mock] t2_event_f(\"%s\", %g)\n",
           marker ? marker : "(null)",
           value);

    return (0);
}

void t2_uninit(void)
{
    printf("[t2-mock] t2_uninit()\n");
}
