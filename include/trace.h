/**
 * @file trace.h
 * @brief Header file for trace logging functions.
 * @author Karol Desnos with the help of MAI-CODE-1.1-Flash
 * @date 2024-06-10
 */

#ifndef TRACE_H
#define TRACE_H

#include <stdint.h>

#ifdef ENABLE_PROFILING
void trace_log_event(const char *phase, const char *func_name, void *caller, long long unsigned int us);
long long unsigned int trace_now_us(void);
#endif

#ifdef ENABLE_PROFILING
// Wrap a selected external call in the same six-field format used by GCC's
// -finstrument-functions hooks: e|x THREAD CORE FUNC CALLER TIMESTAMP.
// The "FUNC" field is a symbol name for manually wrapped calls, which keeps the
// trace readable even for functions that are not present in the executable's
// symbol table or cannot be resolved with addr2line.
// This macro should be used only for external calls that are not instrumented by
// GCC's -finstrument-functions option, otherwise the function will be logged twice,
// with unintended behaviors for nested function calls.
#define TRACE_CALL(func, ...)                                            \
    ({                                                                    \
        uint64_t _trace_begin = trace_now_us();                           \
        void * _trace_caller = __builtin_return_address(0);                \
        const char * _trace_func_name = #func;                             \
        __typeof__(func(__VA_ARGS__)) _trace_result = func(__VA_ARGS__);   \
        uint64_t _trace_end = trace_now_us();                             \
        trace_log_event("e", _trace_func_name, _trace_caller, _trace_begin); \
        trace_log_event("x", _trace_func_name, _trace_caller, _trace_end);   \
        _trace_result;                                                    \
    })
#else
#define TRACE_CALL(func, ...) func(__VA_ARGS__)
#endif    

#endif
