#pragma once

#include "Common.h"
#include <queue>

#undef CURRENT_DAY
#define CURRENT_DAY 6

namespace Day06
{
	template<int rowCount>
	std::string Approach(std::istream& stream)
	{
		std::queue<char> input;
		for (int i = 0; i < rowCount - 1; i++)
		{
			input.push(stream.get());
		}

		int counter = rowCount - 1;
		std::set<char> set;
		while (!stream.eof())
		{
			counter++;
			input.push(stream.get());

			bool isSuccessful = true;
			set.clear();

			for (auto ch : input._Get_container())
			{
				if (!set.insert(ch).second)
				{
					isSuccessful = false;
					break;
				}
			}

			if (isSuccessful)
			{
				return std::to_string(counter);
			}

			input.pop();
		}

		INVALID_INPUT;
	}
}

template<>
std::string PuzzleApproach<6, 1>::RunTest(std::istream& stream)
{
	return Day06::Approach<4>(stream);
}

template<>
std::string PuzzleApproach<6, 2>::RunTest(std::istream& stream)
{
	return Day06::Approach<14>(stream);
}