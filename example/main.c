// Make sure to define MOPI_IMPLEMENTATION BEFORE ITS FIRST INCLUDED
#define MOPI_IMPLEMENTATION
#include "mopi/mopi.h"

// stdio.h for printf
#include <stdio.h>

// A callback function for an inside-look into whats going on
static void _MessageCallback(int level, const char* message)
{
	// A basic "level tagging" system to tell if the message was; fatal, an error, or trace/debug 
	const char* level_tags[] = {
		"[FATAL] ",
		"[ERROR] ",
		"[TRACE] "
	};

	// Actually print the message with some formatting
	printf("%s[mopi] %s\n", level_tags[level+2], message);
}

// _WindowResizeCallback is one of the most important callbacks for most graphics applications
// mopi_Window* window: Is the window that resized
// int width / height: Is the dimensions of the window
// NOTE: only windows that have bound a resize event will call the function,
// ANOTHER NOTE: windows can call the same callback function, just specify 
// 		mopi_SetWindowResizeCallback(window, function) for each window
// ANOTHER ANOTHER NOTE: When a window is minimized the size will change be set to 0
//		i dont know if theres a work around currently but thats how windows does it
//		so make sure you account for that, maybe if the size is [0, 0] disable rendering if thats applicable
static void _WindowResizeCallback(mopi_Window* window, int width, int height)
{
	printf("Resize[%i, %i]\n", width, height);
}

static void _CursorPositionCallback(mopi_Window* window, int x, int y)
{
	printf("CursorPos[%i, %i]\n", x, y);
}

// main function, if you need docs for this... maybe learn C/C++ before using this library
int main(int argc, char** argv)
{
	// Setup messaging callback, this is how you get all feedback, info and error messages
	// from mopi. if something goes wrong the reason causing it will be sent to this callback
	// note: you can and it is recommended to set this up before calling any other mopi functions
	mopi_SetMessageCallback(_MessageCallback);
	
	// Initialises the important parts of mopi
	// It is required to call this to use almost all of mopi
	if (!mopi_Init())
	{
		return -1;
	} 

	// Creates a window and stores it as a pointer to be used later on
	// NOTE: Make sure you always keep this around and NEVER free it on your
	//		own or using mopi_DestroyWindow while you still need it
	mopi_Window* window = mopi_CreateWindow(800, 450, "Hello, world!");

	// Check to make sure the window was actually created
	// If this fails, check with mopi_SetMessageCallback to find out what went wrong
	if (!window)
		return -1;

	// Binds a resize callback to be notified when the window get resized
	mopi_SetWindowResizeCallback(window, _WindowResizeCallback);

	// Binds a cursor position callback to be notified when the cursor changes positions relative to the window
	mopi_SetCursorPositionCallback(window, _CursorPositionCallback);

	// Main application loop
	// This will break if the window has requested to be closed
	// such as pressing the close button or other means of the OS closing the window
	while (!mopi_WindowShouldClose(window))
	{	
		// Updates platform and window
		// NOTE: it is recommended to call this at the end of the frame
		mopi_PollEvents();
	}

	// When you no longer need the window destroy it
	// to free up the memory it takes up
	// NOTE: REMEMBER TO DESTROY THE WINDOW WHEN THE PROGRAM FINISHES!
	mopi_DestroyWindow(window);

	// Destroys and frees all memory used by mopi
	// call this when your program ends, and mopi will no be used again
	mopi_Destroy();
	
	// And finally, return 0 so the program knows it finished without any problems
	// (or atleast any major problems)
	return 0;
}