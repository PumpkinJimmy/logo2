#pragma once
#ifndef _UTILS_H
#define _UTILS_H
#include <array>
#include <cmath>
using namespace std;
inline array<double, 2> move_len(array<double, 2> st, double angle, int len)
{
	array<double, 2> res = st;
	double rad = angle / 180 * 3.141592653589;
	st[0] += len * cos(rad);
	st[1] += len* sin(rad);
	return st;
}

#endif // !_UTILS_H
