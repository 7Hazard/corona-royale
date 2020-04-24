#pragma once

#ifdef CR_CLIENT
#include <SDL_log.h>

#define LogInfo(text, ...) SDL_LogInfo(text, __VA_ARGS__)

#else
#include <stdio.h>

#define LogInfo(text, ...) printf(text, __VA_ARGS__)

#endif
