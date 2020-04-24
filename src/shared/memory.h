#pragma once

#include <memory.h>

#ifdef _WIN32
#include "Windows.h"
#define alloca _alloca
#else
#include <alloca.h>
#endif
