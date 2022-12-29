#pragma once

#include "Common.h"

#undef CURRENT_DAY
#define CURRENT_DAY 4

template<>
std::string PuzzleApproach<4, 1>::RunTest(std::istream& stream)
{
	int counter = 0;

	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);

		size_t elvesOffset = line.find(',');
		size_t firstSeparator = line.find('-');
		size_t secondSeparator = line.find('-', elvesOffset);

		std::pair<int, int> firstElf{ stoi(line.substr(0, firstSeparator)), stoi(line.substr(firstSeparator + 1, elvesOffset - firstSeparator - 1)) };
		std::pair<int, int> secondElf{ stoi(line.substr(elvesOffset + 1, secondSeparator - elvesOffset - 1)), stoi(line.substr(secondSeparator + 1, line.size())) };
		std::pair<int, int> unioned{ std::min(firstElf.first, secondElf.first), std::max(firstElf.second, secondElf.second) };

		if (unioned == firstElf || unioned == secondElf)
		{
			counter++;
		}
	}

	return std::to_string(counter);
}

template<>
std::string PuzzleApproach<4, 2>::RunTest(std::istream& stream)
{
	int counter = 0;

	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);

		size_t elvesOffset = line.find(',');
		size_t firstSeparator = line.find('-');
		size_t secondSeparator = line.find('-', elvesOffset);

		std::pair<int, int> firstElf{ stoi(line.substr(0, firstSeparator)), stoi(line.substr(firstSeparator + 1, elvesOffset - firstSeparator - 1)) };
		std::pair<int, int> secondElf{ stoi(line.substr(elvesOffset + 1, secondSeparator - elvesOffset - 1)), stoi(line.substr(secondSeparator + 1, line.size())) };
		
		if (firstElf.first >= secondElf.first && firstElf.first <= secondElf.second)
		{
			counter++;
		}
		else if (secondElf.first >= firstElf.first && secondElf.first <= firstElf.second)
		{
			counter++;
		}
	}

	return std::to_string(counter);
}