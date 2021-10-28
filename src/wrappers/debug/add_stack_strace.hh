// Created by dominique on 10/27/21.

#pragma once

#include <dlfcn.h>
#include <execinfo.h>
#include <typeinfo>
#include <string>
#include <memory>
#include <cxxabi.h>
#include <cstdlib>

#ifndef NDEBUG
namespace DEBUG {
    /*
     * FOR DEBUGGING PURPOSES *ONLY*: DO NOT USE IN PRODUCTION
     */
    inline void * last_frames[20];
    inline size_t last_size;
    inline std::string exception_name;

    inline std::string demangle(const char *name) {
        int status;
        std::unique_ptr<char,void(*)(void*)> realname(abi::__cxa_demangle(name, 0, 0, &status), &std::free);
        return status ? "failed" : &*realname;
    }
}

extern "C" {
    void __cxa_throw(void *ex, void *info, void (*dest)(void *));
}
#endif