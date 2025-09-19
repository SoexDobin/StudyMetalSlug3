#pragma once

#include "Vector2.h"

typedef struct tagLine
{
	tagLine() : vLeft(Vector2::Zero), vRight(Vector2::Zero) { };
	tagLine(const Vector2& _vLeft, const Vector2& _vRight) : vLeft(_vLeft), vRight(_vRight) { };

	Vector2 vLeft;
	Vector2 vRight;
} LINE;
