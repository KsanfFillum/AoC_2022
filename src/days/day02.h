#pragma once

#include "Common.h"

#include <array>

#undef CURRENT_DAY
#define CURRENT_DAY 2


#define ROCK 1
#define PAPER 2
#define SCISSORS 3

template<>
std::string PuzzleApproach<2, 1>::RunTest(std::istream& stream)
{
	int lookupTable[3][3]
	{
		//					ROCK	PAPER	SCISSORS
		/* ROCK */		{	 1,		  0,		2		},
		/* PAPER */		{	 2,		  1,		0		},
		/* SCISSORS */	{	 0,		  2,		1		}
	};

	int score = 0;

	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);

		char otherShape = line[0] - 'A';
		char myShape = line[2] - 'X';

		score += myShape + 1;
		score += lookupTable[myShape][otherShape] * 3;
	}

	return std::to_string(score);
}

template<>
std::string PuzzleApproach<2, 2>::RunTest(std::istream& stream)
{
	int lookupTable[3][3]
	{
		//	LOSE		DRAW		WIN 
		{	SCISSORS,	ROCK,		PAPER	 },
		{	ROCK,		PAPER,		SCISSORS },
		{	PAPER,		SCISSORS,	ROCK	 }
	};

	int score = 0;

	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);

		char otherShape = line[0] - 'A';
		char myOutcome = line[2] - 'X';

		score += myOutcome * 3;
		score += lookupTable[otherShape][myOutcome];
	}

	return std::to_string(score);
}