#ifndef MOPI_MESSAGE_DEFINES_H
#define MOPI_MESSAGE_DEFINES_H

// pre-written messages for various mopi logs

#define MOPI_MSG_INITIALISE_BEGIN "Initialising mopi systems..."
#define MOPI_MSG_INITIALISE_SUCCESS "Successfully initialised mopi :D"
#define MOPI_MSG_INITIALISE_FAIL_GENERIC "mopi ran into an undefined issue intialising!"

#define MOPI_MSG_WIN32_INITIALISE_HINSTANCE_GET_FAIL "Failed to get HINSTANCE!" // I NEED to rewrite this
#define MOPI_MSG_WIN32_INITIALISE_REGISTER_WINDOW_CLASS_FAIL "Failed to register window class!"

#define MOPI_MSG_DESTROY_BEGIN "Destroying mopi systems..."

#define MOPI_MSG_ERROR_MEMORY "Unable to allocate memory"

#define MOPI_MSG_VECTOR_INDEX_OUT_OF_BOUNDS "Invalid index being accessed on vector!"

#define MOPI_MSG_ATTACH_CALLBACK_RESIZE_FAIL_INVALID_WINDOW "Failed to attach window resize callback! REASON[Window is invalid or not initialised]"

#endif