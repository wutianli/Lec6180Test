
#ifndef _LIB_DEV_H_
#define _LIB_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PRINT_DBG
#define API_PRINT(x...) do{printf("[%s %d] ", __FILE__, __LINE__);printf(x);printf("\n");}while(0)
#else
#define API_PRINT(x...)
#endif

typedef const char cstr;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned short wchar;
#define ulong unsigned int 

#include "serial.h"
#include "libgpio.h"
#include "libat24.h"
#include "libwatchdog.h"

#ifdef __cplusplus
 }
#endif

#endif

