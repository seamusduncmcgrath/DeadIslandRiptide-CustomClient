#pragma once
#include <Windows.h>

#define FILESYSTEM_API __declspec(dllimport)

typedef void(__cdecl* LogPrintCallback)(const char*);
FILESYSTEM_API void __cdecl LogSetPrintCallback(LogPrintCallback callback);