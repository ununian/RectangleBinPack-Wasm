/** @file Rect.cpp
	@author Jukka Jyl�nki

	This work is released to Public Domain, do whatever you want with it.
*/
#include <utility>

#include "GuillotineBinPack.cpp"
#include "MaxRectsBinPack.cpp"
#include "SkylineBinPack.cpp"
#include "ShelfBinPack.cpp"
#include "Rect.cpp"
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

	// GuillotineBinPack.cpp
	enum_<GuillotineBinPack::FreeRectChoiceHeuristic>("GuillotineBinPack_FreeRectChoiceHeuristic")
			.value("RectBestAreaFit", GuillotineBinPack::FreeRectChoiceHeuristic::RectBestAreaFit)
			.value("RectBestLongSideFit", GuillotineBinPack::FreeRectChoiceHeuristic::RectBestLongSideFit)
			.value("RectBestShortSideFit", GuillotineBinPack::FreeRectChoiceHeuristic::RectBestShortSideFit)
			.value("RectWorstAreaFit", GuillotineBinPack::FreeRectChoiceHeuristic::RectWorstAreaFit)
			.value("RectWorstLongSideFit", GuillotineBinPack::FreeRectChoiceHeuristic::RectWorstLongSideFit)
			.value("RectWorstShortSideFit", GuillotineBinPack::FreeRectChoiceHeuristic::RectWorstShortSideFit);

	enum_<GuillotineBinPack::GuillotineSplitHeuristic>("GuillotineBinPack_GuillotineSplitHeuristic")
			.value("SplitShorterLeftoverAxis", GuillotineBinPack::GuillotineSplitHeuristic::SplitShorterLeftoverAxis)
			.value("SplitLongerLeftoverAxis", GuillotineBinPack::GuillotineSplitHeuristic::SplitLongerLeftoverAxis)
			.value("SplitMinimizeArea", GuillotineBinPack::GuillotineSplitHeuristic::SplitMinimizeArea)
			.value("SplitMaximizeArea", GuillotineBinPack::GuillotineSplitHeuristic::SplitMaximizeArea)
			.value("SplitShorterAxis", GuillotineBinPack::GuillotineSplitHeuristic::SplitShorterAxis)
			.value("SplitLongerAxis", GuillotineBinPack::GuillotineSplitHeuristic::SplitLongerAxis);

	class_<GuillotineBinPack>("GuillotineBinPack")
			.constructor<>()
			.constructor<int, int>()
			.function("Init", &GuillotineBinPack::Init)
			.function("Insert_Single",
								select_overload<Rect(int, int, bool, GuillotineBinPack::FreeRectChoiceHeuristic, GuillotineBinPack::GuillotineSplitHeuristic)>(
										&GuillotineBinPack::Insert))
			.function("Insert_Range",
								select_overload<void(vector<RectSize> &, bool, GuillotineBinPack::FreeRectChoiceHeuristic, GuillotineBinPack::GuillotineSplitHeuristic)>(
										&GuillotineBinPack::Insert))
			.function("Occupancy", &GuillotineBinPack::Occupancy)
			.function("GetFreeRectangles", &GuillotineBinPack::GetFreeRectangles)
			.function("GetUsedRectangles", &GuillotineBinPack::GetUsedRectangles)
			.function("MergeFreeList", &GuillotineBinPack::MergeFreeList);

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

	// SkylineBinPack.cpp
	enum_<SkylineBinPack::LevelChoiceHeuristic>("SkylineBinPack_LevelChoiceHeuristic")
			.value("LevelBottomLeft", SkylineBinPack::LevelChoiceHeuristic::LevelBottomLeft)
			.value("LevelMinWasteFit", SkylineBinPack::LevelChoiceHeuristic::LevelMinWasteFit);

	class_<SkylineBinPack>("SkylineBinPack")
			.constructor<>()
			.constructor<int, int, bool>()
			.function("Init", &SkylineBinPack::Init)
			.function("Insert_Range",
								select_overload<void(vector<RectSize> &, vector<Rect> &, SkylineBinPack::LevelChoiceHeuristic)>(
										&SkylineBinPack::Insert))
			.function("Insert_Single",
								select_overload<Rect(int, int, SkylineBinPack::LevelChoiceHeuristic)>(
										&SkylineBinPack::Insert))
			.function("Occupancy", &SkylineBinPack::Occupancy);

	// ShelfBinPack.cpp
	enum_<ShelfBinPack::ShelfChoiceHeuristic>("ShelfBinPack_ShelfChoiceHeuristic")
			.value("ShelfNextFit", ShelfBinPack::ShelfChoiceHeuristic::ShelfNextFit)
			.value("ShelfFirstFit", ShelfBinPack::ShelfChoiceHeuristic::ShelfFirstFit)
			.value("ShelfBestAreaFit", ShelfBinPack::ShelfChoiceHeuristic::ShelfBestAreaFit)
			.value("ShelfWorstAreaFit", ShelfBinPack::ShelfChoiceHeuristic::ShelfWorstAreaFit)
			.value("ShelfBestHeightFit", ShelfBinPack::ShelfChoiceHeuristic::ShelfBestHeightFit)
			.value("ShelfBestWidthFit", ShelfBinPack::ShelfChoiceHeuristic::ShelfBestWidthFit)
			.value("ShelfWorstWidthFit", ShelfBinPack::ShelfChoiceHeuristic::ShelfWorstWidthFit);

	class_<ShelfBinPack>("ShelfBinPack")
			.constructor<>()
			.constructor<int, int, bool>()
			.function("Init", &ShelfBinPack::Init)
			.function("Insert_Single",
								select_overload<Rect(int, int, ShelfBinPack::ShelfChoiceHeuristic)>(
										&ShelfBinPack::Insert))
			.function("Occupancy", &ShelfBinPack::Occupancy);
}