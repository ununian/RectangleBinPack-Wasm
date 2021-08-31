/** @file Rect.cpp
	@author Jukka Jyl�nki

	This work is released to Public Domain, do whatever you want with it.
*/
#include <utility>

// #include "GuillotineBinPack.h"
#include "MaxRectsBinPack.cpp"
#include "Rect.cpp"
// #include "ShelfBinPack.h"
// #include "SkylineBinPack.h"
#include "emscripten/bind.h"

using namespace emscripten;
using namespace std;
using namespace rbp;

EMSCRIPTEN_BINDINGS(c)
{
	register_vector<Rect>("VectorRect");
	register_vector<RectSize>("VectorRectSize");

	// Rect.cpp
	class_<RectSize>("RectSize")
			.constructor<>()
			.constructor<int, int>()
			.property("width", &RectSize::width)
			.property("height", &RectSize::height);

	class_<Rect>("Rect")
			.constructor<>()
			.constructor<int, int, int, int>()
			.property("x", &Rect::x)
			.property("y", &Rect::y)
			.property("width", &Rect::width)
			.property("height", &Rect::height);

	emscripten::function("IsContainedIn", &IsContainedIn);

	// MaxRectsBinPack.cpp
	enum_<MaxRectsBinPack::FreeRectChoiceHeuristic>("MaxRectsBinPack_FreeRectChoiceHeuristic")
			.value("RectBestShortSideFit", MaxRectsBinPack::FreeRectChoiceHeuristic::RectBestShortSideFit)
			.value("RectBestLongSideFit", MaxRectsBinPack::FreeRectChoiceHeuristic::RectBestLongSideFit)
			.value("RectBestAreaFit", MaxRectsBinPack::FreeRectChoiceHeuristic::RectBestAreaFit)
			.value("RectBottomLeftRule", MaxRectsBinPack::FreeRectChoiceHeuristic::RectBottomLeftRule)
			.value("RectContactPointRule", MaxRectsBinPack::FreeRectChoiceHeuristic::RectContactPointRule);

	class_<MaxRectsBinPack>("MaxRectsBinPack")
			.constructor<>()
			.constructor<int, int, bool>()
			.function("Init", &MaxRectsBinPack::Init)
			.function("Insert_Range",
								select_overload<void(vector<RectSize> &, vector<Rect> &, MaxRectsBinPack::FreeRectChoiceHeuristic)>(
										&MaxRectsBinPack::Insert))
			.function("Insert_Single",
								select_overload<Rect(int, int, MaxRectsBinPack::FreeRectChoiceHeuristic)>(
										&MaxRectsBinPack::Insert))
			.function("Occupancy", &MaxRectsBinPack::Occupancy);
}