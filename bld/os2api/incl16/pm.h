/*
    OS/2 Presentation Manager top level include file for 16-bit development.
*/


#ifdef INCL_PM
    #define INCL_BITMAPFILEFORMAT
    #define INCL_DEV
    #define INCL_ERRORS
    #define INCL_GPI
    #define INCL_WIN
#endif

#include <pmwin.h>
#include <pmgpi.h>
#include <pmdev.h>

#ifdef INCL_BITMAPFILEFORMAT
#include <pmbitmap.h>
#endif

