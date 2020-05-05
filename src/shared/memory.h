#pragma once

#include <memory.h>
#include <stdlib.h> 
#include <malloc.h>

#ifdef _WIN32
#include "Windows.h"
#define alloca _alloca
#define malloca _malloca
#define freea _freea
#else
#include <alloca.h>
#endif
