#pragma once

#include <cstdint>
#include <cstring>
#include <cassert>
#include <filesystem>

typedef float float32_t;
typedef double float64_t;

#ifdef WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#ifndef _MFC_VER
#include <windows.h>
#endif
#include <tchar.h>

#include <string>

#define strcasecmp _stricmp
#define localtime_r(_Time, _Tm) localtime_s(_Tm, _Time)
#define gmtime_r(_Time, _Tm) gmtime_s(_Tm, _Time)
#pragma comment(lib, "wsock32.lib")

#define RGBA(r,g,b,a)          (RGB(r, g, b) | (((uint32_t)(uint8_t)(a))<<24))

#ifdef _UNICODE
typedef std::wstring tstring;
typedef std::wstringstream tstringstream;
#define to_tstring(a) std::to_wstring(a)
#else
typedef std::string tstring;
typedef std::stringstream tstringstream;
#define to_tstring(a) std::to_string(a)
#endif

#else
#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <netdb.h>

#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define closesocket close
#define HRESULT long

#define SHARE_AUDIO_SAMPLING kFileFormatPcm32kHzFile
#if PLATFORM_IOS
#define TANGGL_BGRA_EXT GL_RGBA
#elif PLATFORM_ANDROID
#define TANGGL_BGRA_EXT GL_BGRA_EXT
#elif PLATFORM_MAC
#define TANGGL_BGRA_EXT GL_BGRA_EXT
#endif
#endif

#ifndef LOBYTE
#define LOBYTE(w)           ((uint8_t)(((uint32_t)(w)) & 0xff))
#endif

#ifndef GetRValue
#define GetRValue(rgb)      (LOBYTE(rgb))
#endif

#ifndef GetGValue
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#endif

#ifndef GetBValue
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))
#endif

#ifndef PI
#define PI	3.14159265358979323846
#endif