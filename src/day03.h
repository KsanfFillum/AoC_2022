#pragma once

#include "Common.h"

#include <set>
#include <algorithm>
#include <vector>

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
		std::set<char> secondHalf = std::set<char>(line.begin() + halfIndex, line.end());

		std::array<char, 1> dstArr;
		std::set_intersection(firstHalf.begin(), firstHalf.end(), secondHalf.begin(), secondHalf.end(), dstArr.begin());

		score += (dstArr[0] & 0b11111);
		if (!(dstArr[0] & 0b100000))
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
		std::array<std::set<char>, 3> group;
		for (int i = 0; i < 3; i++)
		{
			std::string line;
			std::getline(stream, line);
			group[i] = std::set<char>(line.begin(), line.end());
		}

		static std::vector<char> inter12;
		inter12.resize(std::max(inter12.size(), group[0].size(), group[1].size()));

		auto interEnd = std::set_intersection(group[0].begin(), group[0].end(), group[1].begin(), group[1].end(), inter12.begin());
		
		std::array<char, 1> dstArr;
		std::set_intersection(inter12.begin(), interEnd, group[2].begin(), group[2].end(), dstArr.begin());

		score += (dstArr[0] & 0b11111);
		if (!(dstArr[0] & 0b100000))
		{
			score += 26;
		}
	}

	return std::to_string(score);
}