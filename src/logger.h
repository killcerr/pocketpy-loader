#pragma once
#include <ll/api/Logger.h>
template <::std::size_t L>
struct CStr {
    char          data[L];
    ::std::size_t length = L;
};

template <::std::size_t L>
CStr(const char (&)[L]) -> CStr<L>;
template <CStr filename>
struct FileLogger : ll::Logger {
    using ll::Logger::Logger;
    FileLogger() : ll::Logger(filename.data) {}
};
#define FILE_LOGGER() FileLogger<CStr{__FILE__}> logger(__FILE__)