#pragma once

#include "Common.h"

#include <set>
#include <algorithm>
#include <vector>

#undef CURRENT_DAY
#define CURRENT_DAY 3

template<>
std::string PuzzleApproach<3, 1>::RunTest(std::istream& stream)
{
	int score = 0;
	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);
		
		const size_t halfIndex = line.size() / 2;

		std::set<char> firstHalf = std::set<char>(line.begin(), line.begin() + halfIndex);
		char intersectedChar = '/0';
		for (auto it = line.begin() + halfIndex; it < line.end(); ++it)
		{
			if (firstHalf.count(*it))
			{
				intersectedChar = *it;
				break;
			}
		}
		// point here is in ASCII formatting
		// A in ASCII is 65 (1000001)
		// a is 97 (1100001)
		score += (intersectedChar & 0b11111);
		if (!(intersectedChar & 0b100000))
		{
			score += 26;
		}
	}

	return std::to_string(score);
}

template<>
std::string PuzzleApproach<3, 2>::RunTest(std::istream& stream)
{
	int score = 0;
	while (!stream.eof())
	{

		std::string line1, line2;
		std::getline(stream, line1);

		std::set<char> originSet = std::set<char>(line1.begin(), line1.end());
		std::getline(stream, line1);
		std::getline(stream, line2);

		std::set<char> secondSet;
		for (auto ch : line1)
		{
			if (originSet.find(ch) != originSet.end())
			{
				secondSet.insert(ch);
			}
		}

		char intersectedChar;
		for (auto ch : line2)
		{
			if (secondSet.find(ch) != secondSet.end())
			{
				intersectedChar = ch;
				break;
			}
		}

		score += (intersectedChar & 0b11111);
		if (!(intersectedChar & 0b100000))
		{
			score += 26;
		}
	}

	return std::to_string(score);
}