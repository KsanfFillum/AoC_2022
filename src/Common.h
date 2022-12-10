#pragma once

#define NOT_IMPLEMENTED_PUZZLE return "Not Implemented"
#define INVALID_INPUT return "Invalid Input or Processing"

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
