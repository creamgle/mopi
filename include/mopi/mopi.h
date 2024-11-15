#pragma once

/**
    Figure out which OS we are using
*/
#ifdef __APPLE__
    #include "TargetConditionals.h"

    #if TARGET_OS_MAC == 1
        #define MOPI_PLATFORM_MAC
    #else
        #error "mopi: The only supported AppleOS is macOS!"
    #endif
#endif

/**
    Define MOPI_API
    -- Users will probably never use this definintion
        only intended for use of mopi source
*/
#ifdef MOPI_EXPORT
    #ifdef MOPI_PLATFORM_MAC
        #define MOPI_API __attribute__((visibility("default")))
    #endif
#else
    #ifdef MOPI_PLATFORM_MAC
        #define MOPI_API __attribute__((visibility("hidden")))
    #endif
#endif

// This could result in compiler issues if other librarys dont define 'BOOL' as 
// an signed char, i should probably look for a better way to define a boolean
typedef signed char MOPI_BOOL;

#ifndef FALSE
	#define FALSE (signed char)0
#endif

#ifndef TRUE
	#define TRUE (signed char)1
#endif

/* Log specific defines
 * 	use '#define MOPI_LOG_LEVEL_... X"
 * 	where 'X' is an integer value matched to the
 * 	users log level
 */

#ifndef MOPI_LOG_LEVEL_FATAL
	#define MOPI_LOG_LEVEL_FATAL 0
#endif

#ifndef MOPI_LOG_LEVEL_ERROR
	#define MOPI_LOG_LEVEL_ERROR 1
#endif

#ifndef MOPI_LOG_LEVEL_WARNING
	#define MOPI_LOG_LEVEL_WARNING 2
#endif

#ifndef MOPI_LOG_LEVEL_TRACE
	#define MOPI_LOG_LEVEL_TRACE 3
#endif

#ifndef MOPI_LOG_LEVEL_INFO

	#define MOPI_LOG_LEVEL_INFO 4
#endif

#ifndef MOPI_LOG_LEVEL_DEBUG
	#define MOPI_LOG_LEVEL_DEBUG 5
#endif

/* Platform specific window handles, the end user does not interact with this 
 * * Should be defined in a source file */
struct mopi_WindowHandle;

/* Generic window struct, this is what the user interacts with */
typedef struct mopi_Window {
	int width;
	int height;

	const char *title;

	struct mopi_WindowHandle *handle;
} mopi_Window;

MOPI_API MOPI_BOOL mopi_Init(void);
MOPI_API void mopi_Destroy(void);

MOPI_API void mopi_Log(int level, const char *message, ...);

/* Windowing */
MOPI_API mopi_Window* mopi_CreateWindow(int width, int height, const char *title);
MOPI_API void mopi_DestroyWindow(mopi_Window *window);

MOPI_API void mopi_PollEvents();
MOPI_API MOPI_BOOL mopi_WindowShouldClose(mopi_Window *window);

/* From here down is for mopi dev only, mostly debugging stuff */
#ifdef MOPI_DEBUG
    #define MOPI_DPRINT(message, ...) mopi_Log(MOPI_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
    #define MOPI_DPRINT(message, ...)
#endif
