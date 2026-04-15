#pragma once

#ifdef SLV_EXPORTS
#define SLV_DLL __declspec(dllexport)
#else
#define SLV_DLL __declspec(dllimport)
#endif

#ifdef _WIN32
#define SLV_COLORED_LOGS 1
#else
// SLV is yet to support other platforms
#define SLV_COLORED_LOGS 0
#endif