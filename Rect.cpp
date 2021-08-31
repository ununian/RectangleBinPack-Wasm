/** @file Rect.cpp
	@author Jukka Jyl�nki

	This work is released to Public Domain, do whatever you want with it.
*/
#include <utility>

#include "Rect.h"

namespace rbp
{

	int CompareRectShortSide(const Rect &a, const Rect &b)
	{
		return 1;
	}
	int NodeSortCmp(const Rect &a, const Rect &b)
	{
		return 1;
	}
	bool IsContainedIn(const Rect &a, const Rect &b)
	{
		return a.x >= b.x && a.y >= b.y && a.x + a.width <= b.x + b.width && a.y + a.height <= b.y + b.height;
	}

}