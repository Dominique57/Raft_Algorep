// Created by dominique on 10/27/21.

#include "add_stack_strace.hh"

#ifndef NDEBUG
extern "C" {
void __cxa_throw(void *ex, void *info, void (*dest)(void *)) {
    DEBUG::exception_name = DEBUG::demangle(reinterpret_cast<const std::type_info *>(info)->name());
    DEBUG::last_size = backtrace(DEBUG::last_frames, sizeof DEBUG::last_frames / sizeof(void *));
    backtrace_symbols_fd(DEBUG::last_frames, DEBUG::last_size, 2);
    static void (*const rethrow)(void *, void *, void (*)(void *)) __attribute__((noreturn)) = (void (*)(void *, void *, void (*)(void *))) dlsym(RTLD_NEXT, "__cxa_throw");
    rethrow(ex, info, dest);
}
}
#endif