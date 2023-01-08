#pragma once

#define NOT_IMPLEMENTED_PUZZLE return "Not Implemented"
#define INVALID_INPUT return "Invalid Input or Processing"

template<int day>
struct NeedShowDayResults
{
#if SEE_ALL_DAYS_RESULT
	constexpr static bool Value = true;
#else
	constexpr static bool Value = false;
#endif
};

namespace RunPuzzleTraitsConstant
{
	constexpr char RunTest = 0b01;
	constexpr char RunMain = 0b10;
}

template<int day>
struct RunPuzzleTraits
{
	constexpr static char Value = RunPuzzleTraitsConstant::RunTest | RunPuzzleTraitsConstant::RunMain;
};

#include <iostream>

template<int day, int puzzle>
void Run();

template<int day, int puzzle>
struct PuzzleApproach
{
	inline std::string RunTest(std::istream& stream);
};

template<int day, int puzzle>
std::string PuzzleApproach<day, puzzle>::RunTest(std::istream& stream)
{
	NOT_IMPLEMENTED_PUZZLE;
}

#include "days/day01.h"
#include "days/day02.h"
#include "days/day03.h"
#include "days/day04.h"
#include "days/day05.h"
#include "days/day06.h"
#include "days/day07.h"
#include "days/day08.h"
#include "days/day09.h"
#include "days/day10.h"
#include "days/day11.h"
#include "days/day12.h"
#include "days/day13.h"
#include "days/day14.h"
#include "days/day15.h"
#include "days/day16.h"
#include "days/day17.h"
