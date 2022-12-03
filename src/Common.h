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

#include "day01.h"
#include "day02.h"
#include "day03.h"
#include "day04.h"