/**
 * @file trace.c
 * @brief Trace functions for profiling
 * @author Karol Desnos
 * Code partially inspired from https://balau82.wordpress.com/2010/10/06/trace-and-profile-function-calls-with-gcc/
 * Coded with copilot the help of MAI-CODE-1.1-Flash
 */

#ifdef _WIN32
#include <windows.h>
#define getThreadId() GetCurrentThreadId()
#define getCoreId() (unsigned int)GetCurrentProcessorNumber()
#else
#define _GNU_SOURCE // for gettid() and sched_getcpu()
#include <unistd.h>
#include <sched.h>
#define getThreadId() (unsigned long)gettid()
#define getCoreId() (unsigned int)sched_getcpu()
#endif

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "trace.h"

#ifdef ENABLE_PROFILING
static FILE *fp_trace;

long long unsigned int __attribute__((no_instrument_function))
trace_now_us (void)
{
#ifdef _WIN32
    static LARGE_INTEGER frequency = {0};
    LARGE_INTEGER now;

    if (frequency.QuadPart == 0)
        QueryPerformanceFrequency(&frequency);

    QueryPerformanceCounter(&now);
    return (uint64_t)((now.QuadPart * 1000000ULL) / frequency.QuadPart);
#else
    struct timespec ts;

    if (timespec_get(&ts, TIME_UTC) == TIME_UTC)
        return (uint64_t)ts.tv_sec * 1000000ULL +
               (uint64_t)ts.tv_nsec / 1000ULL;
#endif

    return 0ULL;
}

void __attribute__((no_instrument_function))
trace_log_event(const char *phase, const char *func_name, void *caller, long long unsigned int us)
{
    if (fp_trace != NULL) {
        fprintf(fp_trace, "%s %lu %u %s %p %llu\n",
                phase,
                (unsigned long)getThreadId(),
                getCoreId(),
                func_name,
                caller,
                us);
    }
}

void __attribute__((no_instrument_function, constructor))
trace_begin (void)
{
 fp_trace = fopen("trace.out", "w");
}
 
void __attribute__((no_instrument_function, destructor))
trace_end (void)
{
 if(fp_trace != NULL) {
 fclose(fp_trace);
 }
}
 
void __attribute__((no_instrument_function))
__cyg_profile_func_enter (void *func,  void *caller)
{
 if(fp_trace != NULL) {
 fprintf(fp_trace, "e %lu %u %p %p %llu\n",
         (unsigned long)getThreadId(), getCoreId(), func, caller, trace_now_us());
 }
}
 
void __attribute__((no_instrument_function))
__cyg_profile_func_exit (void *func, void *caller)
{
 if(fp_trace != NULL) {
 fprintf(fp_trace, "x %lu %u %p %p %llu\n",
         (unsigned long)getThreadId(), getCoreId(), func, caller, trace_now_us());
 }
}
#endif