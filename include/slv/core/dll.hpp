#pragma once

#ifdef SLV_EXPORTS
#define SLV_DLL __declspec(dllexport)
#else
#define SLV_DLL __declspec(dllimport)
#endif