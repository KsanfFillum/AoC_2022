#pragma once

#include "Common.h"
#include <string>
#include <array>

#undef CURRENT_DAY
#define CURRENT_DAY 1

void CompareAndFlush(int& bestValue, int& currentValue)
{
	if (currentValue > bestValue)
	{
		bestValue = currentValue;
	}

	currentValue = 0;
}

template<>
std::string PuzzleApproach<1, 1>::RunTest(std::istream& stream)
{
	int bestValue = 0;
	int currentValue = 0;

	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);

		if (line.empty())
		{
			CompareAndFlush(bestValue, currentValue);
		}
		else 
		{
			currentValue += stoi(line);
		}
	}

	CompareAndFlush(bestValue, currentValue);
	return std::to_string(bestValue);
}

void CompareAndFlush(std::array<int, 3>& bestValues, int& currentValue)
{
	if (currentValue >= bestValues[0])
	{
		bestValues[2] = bestValues[1];
		bestValues[1] = bestValues[0];
		bestValues[0] = currentValue;
	}
	else if (currentValue >= bestValues[1])
	{
		bestValues[2] = bestValues[1];
		bestValues[1] = currentValue;
	}
	else if (currentValue > bestValues[2])
	{
		bestValues[2] = currentValue;
	}

	currentValue = 0;
}

template<>
std::string PuzzleApproach<1, 2>::RunTest(std::istream& stream)
{
	std::array<int, 3> bestValues{ 0, 0, 0 };
	int currentValue = 0;

	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);

		if (line.empty())
		{
			CompareAndFlush(bestValues, currentValue);
		}
		else
		{
			currentValue += stoi(line);
		}
	}

	CompareAndFlush(bestValues, currentValue);
	const int sum = bestValues[0] + bestValues[1] + bestValues[2];
	return std::to_string(sum);
}