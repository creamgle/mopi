#ifndef MOPI_H
#define MOPI_H

/* Platform detection */
#if defined(_WIN32) || defined(WIN32)
    #define MOPI_PLATFORM_WIN32
#else
    #error "There is not support for your current platform!"
#endif

#ifdef MOPI_IMPLEMENTATION
    #ifdef MOPI_PLATFORM_WIN32
        #define MOPI_API __declspec(dllimport)
    #endif
#else
    #ifdef MOPI_PLATFORM_WIN32
        #define MOPI_API __declspec(dllexport)
    #endif
#endif

/* 
    MOPI_MESSAGE_FATAL is for message callbacks that will result in mopi fatal errors
    (crashes, memory errors, requirement issues, mis-configurations, and other stuff will
        will prevent mopi from running at all)
*/
#define MOPI_MESSAGE_FATAL -2

/*
    MOPI_MESSAGE_ERROR is for message callbacks that will result in mopi errors
    (major errors to minor errors, the program can still run, but it wont be as expected,
        errors can be fatal, but most of the time arent
*/
#define MOPI_MESSAGE_ERROR -1

/*
    MOPI_MESSAGE_TRACE is for message callbacks for info on what mopi is doing,
    what it will do, and just other info
*/
#define MOPI_MESSAGE_TRACE  0

/*
    The same as FALSE but for mopi use
*/
#define MOPI_FALSE 0
#define MOPI_FAIL 0
/*
    The same as TRUE but for mopi use
*/
#define MOPI_TRUE 1
#define MOPI_OKAY 1
#define MOPI_PASS 1

typedef int MOPI_BOOL;

struct mopi_WindowCallbacks {
    void (*cbfun_resize)(struct mopi_Window* window, int width, int height);
    void (*cbfun_cursorpos)(struct mopi_Window* window, int x, int y);
};

typedef struct mopi_Window {
    struct mopi_WindowCallbacks callbacks;
    struct _WindowInternal* internal;
} mopi_Window;

/**
 * @brief Initialises mopi, must be called for most functions to work
 * @return MOPI_FAIL[0] / MOPI_PASS[1]; did it succeed? 
 */
MOPI_API int mopi_Init(void);

/**
 * @brief Destroys and frees all used memory @endparblock
 * It is very import you make sure to call mopi_Destroy()
 * when mopi is not going to be used again in your code
 * failure to destroy will result in unpredictable behaviour
 * and possible memory de-allocation issues depending on OS / compiler
 */
MOPI_API void mopi_Destroy(void);

/**
 * @brief Creates a window and shows it
 * 
 * @param [int]width The starting window width
 * @param [int]height The starting window height
 * @param [int]title The windows title
 * @return mopi_Window* Window struct 
 */
MOPI_API mopi_Window* mopi_CreateWindow(int width, int height, const char* title);

/**
 * @brief Destroys and de-allocates all memory associated with the window specified
 * 
 * @param [mopi_Window*]window The window to be destroyed 
 */
MOPI_API void mopi_DestroyWindow(mopi_Window* window);

/**
 * @brief Checks if specified window has sent a close request
 * 
 * @param [mopi_Window*]window The window to check for close request
 * @returns MOPI_FALSE[0] / MOPI_TRUE[1]; Should the window close?
 */
MOPI_API MOPI_BOOL mopi_WindowShouldClose(mopi_Window* window);

/** 
 * @brief Updates OS-platform stuff(Windowing, Input, Time, etc...)
*/
MOPI_API void mopi_PollEvents();

/**
 * @brief Sets specified window's size
 * 
 * @param [mopi_Window*]window The window you want to change the size of
 * @param [int]width The width to change it too
 * @param [int]height The height to change it too
 */
MOPI_API void mopi_SetWindowSize(mopi_Window* window, int width, int height);

/**
 * @brief Assign a resize callback to get notified when a window resizes
 *
 * @param [mopi_Window*]window The window that should check for a window resize event
 * @param [function]cbfun_resize The function that gets called when the specified window gets resized
 */
MOPI_API void mopi_SetWindowResizeCallback(mopi_Window* window, void (*cbfun_resize)(mopi_Window* window, int width, int height));

/**
 * @brief Maximized the window to take up the full display while still in windowed mode
 * 
 * @param [mopi_Window*]window The window to be maximized 
 */
MOPI_API void mopi_WindowMaximize(mopi_Window* window);

/**
 * @brief Minimized window to dock, taskbar or whatever your os uses
 * 
 * @param [mopi_Window*]window The window to be minimized 
 */
MOPI_API void mopi_WindowMinimize(mopi_Window* window);

/**
 * @brief Retrieves the cursor position relative to the window
 * 
 * @param [mopi_Window*]window The window to get the cursor relative too
 * @param [int*]out_x The integer pointer to store the x-position
 * @param [int*]out_y The integer pointer to store the y-position
 */
MOPI_API void mopi_GetCursorPosition(mopi_Window* window, int* out_x, int* out_y);

/**
 * @brief Assigns a cursor position callback to get notiffied when the cursor position changes
 * 
 * @param [mopi_Window*]window The window that should check for a cursor position change
 * @param [function]cbfun_cursorpos The function that gets called when the specified window's cursor moves
 */
MOPI_API void mopi_SetCursorPositionCallback(mopi_Window* window, void (*cbfun_cursorpos)(mopi_Window* window, int x, int y));

/**
 * @brief Assign a message callback to get an inside look into debug-info and errors
 * 
 * @param [function]cbfun_message A pointer to the function that gets called when mopi sends a message 
 * @return MOPI_API 
 */
MOPI_API void mopi_SetMessageCallback(void (*cbfun_message)(int level, const char* message));


#endif
