//============================================================================
// Name        : llUtils.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <stdarg.h>
#include "cobUtils.h"

#ifdef WIN32
#include <windows.h>
#endif

NS_COB_BEGIN

const char* LL_STRING_EMPTY = "";

void Utils::__log(const char *format,...)
{
    char strBuffer[4096] = { 0 };
    va_list vlArgs;
    va_start(vlArgs, format);
    vsnprintf(strBuffer, sizeof(strBuffer) - 1, format, vlArgs);
    va_end(vlArgs);

#ifdef WIN32
    OutputDebugString(strBuffer);
#else
    std::cout << strBuffer << std::endl;
#endif
}

void Utils::__assert(const char* file, int lineno, const char* format,...)
{
    char strBuffer[4096] = { 0 };
    va_list vlArgs;
    va_start(vlArgs, format);
    vsnprintf(strBuffer, sizeof(strBuffer) - 1, format, vlArgs);
    va_end(vlArgs);

    COBLOG("Assert failed: %s\n[%s:%d]\n", strBuffer, file, lineno);
    abort();
}

NS_COB_END
