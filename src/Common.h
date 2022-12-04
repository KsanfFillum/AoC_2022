#pragma once

#define NOT_IMPLEMENTED_PUZZLE return "Not Implemented"

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
