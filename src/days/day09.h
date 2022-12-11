#pragma once

#include "Common.h"
#include "Helpers.h"

namespace Day09
{
	// allow only numbers [0; 65535] for 4byte-int
	struct VectorComparator
	{
		inline bool operator()(const Vector2& lhv, const Vector2 rhv) const
		{
			constexpr size_t intSize = sizeof(int);
			return ((lhv.X << intSize * 4) + lhv.Y) < ((rhv.X << intSize * 4) + rhv.Y);
		}
	};

	inline int Sign(int val)
	{
		return (0 < val) - (val < 0);
	}

	template<int knotsCount>
	std::string Approach(std::istream& stream)
	{
		std::set<Vector2, VectorComparator> visitedPoints;

		std::array<Vector2, knotsCount> knots;
		visitedPoints.insert(knots[0]);

		std::map<char, Vector2> directions
		{
			{ 'R', {  1,  0 }},
			{ 'L', { -1,  0 }},
			{ 'U', {  0,  1 }},
			{ 'D', {  0, -1 }}
		};

		while (!stream.eof())
		{
			std::string line;
			std::getline(stream, line);

			Vector2 moveDir = directions[line[0]];
			int moveLength = stoi(line.substr(2));

			while (moveLength-- > 0)
			{
				knots[0] = knots[0] + moveDir;

				for (int i = 1; i < knotsCount; i++)
				{
					Vector2 diff = knots[i - 1] - knots[i];
					if (std::abs(diff.X) > 1 || std::abs(diff.Y) > 1)
					{
						knots[i] = knots[i] + Vector2(Sign(diff.X), Sign(diff.Y));
					}
				}

				visitedPoints.insert(knots[knotsCount - 1]);
			}
		}

		return std::to_string(visitedPoints.size());
	}
}

std::string PuzzleApproach<9, 1>::RunTest(std::istream& stream)
{
	return Day09::Approach<2>(stream);
}

std::string PuzzleApproach<9, 2>::RunTest(std::istream& stream)
{
	return Day09::Approach<10>(stream);
}