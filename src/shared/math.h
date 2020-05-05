#pragma once

#include <stdio.h>
#include <math.h>

inline double distance(double x1, double y1, double x2, double y2)
{
    return hypot(x2 - x1, y2 - y1);;
}
