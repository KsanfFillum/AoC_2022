#pragma once

#include "Common.h"
#include <functional>

namespace Day08
{
	constexpr char NorthBit	= 0b0001;
	constexpr char EastBit	= 0b0010;
	constexpr char SouthBit	= 0b0100;
	constexpr char WestBit	= 0b1000;

	struct Vector2
	{
		Vector2(size_t x, size_t y)
			: X(x), Y(y)
		{
		}

		Vector2()
			: Vector2(0, 0)
		{
		}

		size_t X;
		size_t Y;
	};

	struct Tree
	{
		Tree() = default;

		Tree(int height)
			: Height(height), Visibility(0)
		{
		}

		int Height;
		char Visibility;
	};

	inline void ProcessLine(std::vector<Tree>& map, const std::string& line)
	{
		for (const char ch : line)
		{
			map.emplace_back(atoi(&ch));
		}
	}

	struct Map
	{
		Map(std::istream& stream)
		{
			std::string line;
			std::getline(stream, line);

			Width = line.size();
			Height = 1;
			TreeMap.reserve(Width * Width);
			ProcessLine(TreeMap, line);

			while (!stream.eof())
			{
				std::getline(stream, line);
				ProcessLine(TreeMap, line);
				Height++;
			};
		}

		Tree at(size_t x, size_t y) const { return TreeMap[x + y * Width]; }
		void foreach(std::function<void(Tree&, Vector2)> func) 
		{
			size_t index = 0;

			for (size_t y = 0; y < Height; y++)
			{
				for (size_t x = 0; x < Width; x++)
				{
					Vector2 coord(x, y);
					func(TreeMap[index++], coord);
				}
			}
		}

		void foreach_reversed(std::function<void(Tree&, Vector2)> func)
		{
			size_t index = Width * Height;

			for (int y = Height - 1; y >= 0; y--)
			{
				for (int x = Width - 1; x >= 0; x--)
				{
					Vector2 coord(x, y);
					func(TreeMap[--index], coord);
				}
			}
		}

		size_t GetHeight() const { return Height; }
		size_t GetWidth() const { return Width; }

	private:
		std::vector<Tree> TreeMap;

		size_t Width;
		size_t Height;
	};

	struct Puzzle1Processor
	{
		void ForeachFunc(Tree& tree, Vector2 index)
		{
			if (Horizontal[index.Y] < tree.Height)
			{
				tree.Visibility |= HorizontalBit;
				Horizontal[index.Y] = tree.Height;
			}

			if (Vertical[index.X] < tree.Height)
			{
				tree.Visibility |= VerticalBit;
				Vertical[index.X] = tree.Height;
			}
		}

		void Setup(const Map& map, char horizontalBit, char verticalBit)
		{
			Horizontal.clear();
			Horizontal.resize(map.GetHeight(), -1);
			Vertical.clear();
			Vertical.resize(map.GetWidth(), -1);

			HorizontalBit = horizontalBit;
			VerticalBit = verticalBit;
		}

	private:
		std::vector<int> Horizontal;
		std::vector<int> Vertical;
		
		char HorizontalBit;
		char VerticalBit;
	};

}

std::string PuzzleApproach<8, 1>::RunTest(std::istream& stream)
{
	using namespace Day08;

	Map map(stream);

	Puzzle1Processor processor;

	processor.Setup(map, WestBit, NorthBit);
	map.foreach(std::bind(&Puzzle1Processor::ForeachFunc, &processor, std::placeholders::_1, std::placeholders::_2));

	processor.Setup(map, EastBit, SouthBit);
	map.foreach_reversed(std::bind(&Puzzle1Processor::ForeachFunc, &processor, std::placeholders::_1, std::placeholders::_2));

	size_t counter = 0;
	map.foreach([&](Tree& tree, Vector2) 
		{  
			counter += static_cast<size_t>(static_cast<bool>(tree.Visibility)); 
		});

	return std::to_string(counter);
}

std::string PuzzleApproach<8, 2>::RunTest(std::istream& stream)
{
	using namespace Day08;

	Map map(stream);

	size_t highestScore = 0;
	map.foreach([&](Tree& tree, Vector2 coord)
		{
			if (coord.X == 0 || coord.Y == 0)
			{
				return;
			}

			size_t score = 1;
			size_t counter = 0;
			const int height = tree.Height;
			for (int x = coord.X + 1; x < map.GetWidth(); x++)
			{
				counter++;
				if (map.at(x, coord.Y).Height >= height)
				{
					break;
				}
			}
			score *= counter;
			counter = 0;

			for (int x = coord.X - 1; x != std::numeric_limits<size_t>::max(); x--)
			{
				counter++;
				if (map.at(x, coord.Y).Height >= height)
				{
					break;
				}
			}
			score *= counter;
			counter = 0;

			for (int y = coord.Y + 1; y < map.GetHeight(); y++)
			{
				counter++;
				if (map.at(coord.X, y).Height >= height)
				{
					break;
				}
			}
			score *= counter;
			counter = 0;

			for (int y = coord.Y - 1; y != std::numeric_limits<size_t>::max(); y--)
			{
				counter++;
				if (map.at(coord.X, y).Height >= height)
				{
					break;
				}
			}
			score *= counter;
			counter = 0;

			if (score > highestScore)
			{
				highestScore = score;
			}
		});

	return std::to_string(highestScore);
}