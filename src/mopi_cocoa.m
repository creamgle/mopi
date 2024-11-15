#include "mopi/mopi.h"
#include <AppKit/AppKit.h>
#include <Foundation/Foundation.h>

#ifdef MOPI_PLATFORM_MAC
#include <Cocoa/Cocoa.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

@interface AppDelegate : NSObject<NSApplicationDelegate>
@end

@interface WindowDelegate : NSObject<NSWindowDelegate> {
	@public
		mopi_Window *mopi_window;
}

- (instancetype)initWithState:(mopi_Window*)init_state;

@end

static struct mopiCocoa_Context {
	NSApplication *application;
	AppDelegate *app_delegate;
} *mopiCocoa_Context;

typedef struct mopi_WindowHandle {
	WindowDelegate *delegate;
	NSWindow *window;
	BOOL close_requested;
} mopi_WindowHandle;

@implementation WindowDelegate
	
- (instancetype)initWithState:(mopi_Window *)init_state {
	self = [super init];

	if (self != nil)
	{
		mopi_window = init_state;
	}

	return self;
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
	if (!sender)
	{
		mopi_Log(MOPI_LOG_LEVEL_ERROR, "[mopi] Who tf did i get windowShouldClose requested from");
		return FALSE;
	}

	mopi_Window *window = ((WindowDelegate*)sender.delegate)->mopi_window;
	if (!window)
	{
		return FALSE;
	}

	window->handle->close_requested = YES;

	return YES;
}

@end


MOPI_BOOL mopi_Init()
{
    MOPI_DPRINT("[mopi] Initialising osx platform...");
	
	mopiCocoa_Context = malloc(sizeof(struct mopiCocoa_Context));

	mopiCocoa_Context->application = [NSApplication sharedApplication];
	if (!mopiCocoa_Context->application)
	{
		mopi_Log(MOPI_LOG_LEVEL_FATAL, "[mopi] Failed to create NSApplication!");
		return FALSE;
	}

	[mopiCocoa_Context->application setActivationPolicy:NSApplicationActivationPolicyRegular];
	[mopiCocoa_Context->application activateIgnoringOtherApps:YES];

	MOPI_DPRINT("[mopi] Successfully initialised all mopi systems :D");

    return TRUE;
}

void mopi_Destroy()
{
    MOPI_DPRINT("[mopi] Destroying mopi systems...");
	free(mopiCocoa_Context->app_delegate);
	free(mopiCocoa_Context->application);
}

void mopi_Log(int level, const char *message, ...)
{
	char out_message[32000];
	memset(out_message, 0, sizeof(out_message));
	
	__builtin_va_list arg_ptr;
	va_start(arg_ptr, message);
	vsnprintf(out_message, 32000, message, arg_ptr);
	va_end(arg_ptr);

	char out_message2[32000];
	sprintf(out_message2, "%s%s\n", "", out_message);

	printf("%s", out_message2);
}

mopi_Window* mopi_CreateWindow(int width, int height, const char *title)
{
	mopi_Window *window = malloc(sizeof(mopi_Window));
	window->width = width;
	window->height = height;
	window->title = title;

	window->handle = malloc(sizeof(mopi_WindowHandle));

	MOPI_DPRINT("[mopi] Created basic mopi_Window* struct");

	NSRect client_size = NSMakeRect(100, 100, width, height);
	NSUInteger style_flag = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable;

	MOPI_DPRINT("[mopi] Created basic info and flags for creating Cocoa window");

	window->handle->window = [[NSWindow alloc] initWithContentRect:client_size
													styleMask:style_flag
														backing:NSBackingStoreBuffered
															defer:NO];

	MOPI_DPRINT("[mopi] Create NSWindow* inside of mopi_Window->(internal handle)->NSWindow");

	if (!window->handle->window)
	{
		mopi_Log(MOPI_LOG_LEVEL_FATAL, "[mopi] Failed to create window!");
		return NULL;
	}
	
	window->handle->delegate = [[WindowDelegate alloc] initWithState:window];
	if (!window->handle->delegate)
	{
		mopi_Log(MOPI_LOG_LEVEL_FATAL, "[mopi] Failed to create window delegate!");
		return NULL;
	}

	MOPI_DPRINT("[mopi] Successfully created window delegate :D");
	MOPI_DPRINT("[mopi] All window validation was sucessfull");

	[window->handle->window setTitle:@"Test"];
	MOPI_DPRINT("[mopi] Set window title to[%s]", title);
	[window->handle->window makeKeyAndOrderFront:nil];
	[window->handle->window setDelegate:window->handle->delegate];

	MOPI_DPRINT("[mopi] Sucesffuly cvreated mopi window :D");

	return window;
}

void mopi_DestroyWindow(mopi_Window *window)
{
	free(window);
	free(window->handle);
}

void mopi_PollEvents()
{
	NSEvent *event = [mopiCocoa_Context->application nextEventMatchingMask:NSEventMaskAny 
														untilDate:[NSDate distantFuture] 
															inMode:NSDefaultRunLoopMode 
																dequeue:YES];
	[mopiCocoa_Context->application sendEvent:event];
	[mopiCocoa_Context->application updateWindows];
}

MOPI_BOOL mopi_WindowShouldClose(mopi_Window *window)
{
	return window->handle->close_requested;
}

#endif
