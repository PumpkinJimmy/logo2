#pragma once
#ifndef _CONTEXT_H
#define _CONTEXT_H
#include <array>
using namespace std;
struct Turtle
{
	array<int, 3> color;
	array<double, 2> pos;
	bool is_cloak;
	int angle;
	Turtle() : is_cloak(false), angle(0) {}
};

#endif