#pragma once

#ifdef CRX_EXPORTS
#define CRX_DLL __declspec(dllexport)
#else
#define CRX_DLL __declspec(dllimport)
#endif

#ifdef _WIN32
#define CRX_COLORED_LOGS 1
#else
// cruxus is yet to support other platforms
#define CRX_COLORED_LOGS 0
#endif