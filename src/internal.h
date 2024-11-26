#ifndef MOPI_INTERNAL_H
#define MOPI_INTERNAL_H

#include "mopi/mopi.h"
#include "message_defines.h"

static struct {
	void (*cbfun_message)(int level, const char* message);
} internal_callbacks;

MOPI_API void mopiInternal_SendMessage(int level, const char* message);

#endif