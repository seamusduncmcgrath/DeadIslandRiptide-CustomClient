#pragma once
#include <Windows.h>

#define FILESYSTEM_API __declspec(dllimport)

class FFSAddSourceFlags {
public:
	enum ENUM {
		SUBDIRS = 0x1,
		APPEND = 0x2,
		STRIP_LAST_DIR = 0x4,
		BROWSABLE = 0x8,
		ALLOW_DUPLICATES = 0x10,
		PRELOAD = 0x20,
	};
};

namespace fs
{
	FILESYSTEM_API bool __cdecl init(const char* write_path, int write_path_flags, const char* cache_dir, bool fallback, bool use_fs_cache);
	FILESYSTEM_API bool __cdecl add_source(const char* source_path, int flags);
	FILESYSTEM_API void __cdecl shutdown();
}
