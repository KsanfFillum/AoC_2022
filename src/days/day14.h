#pragma once

#include "Common.h"
#include "Helpers.h"

#undef CURRENT_DAY
#define CURRENT_DAY 14

namespace Day14
{
	struct Map
	{
		using Container = std::vector<std::vector<bool>>;

	public:

		Map(std::istream& str, bool isFirstPuzzle);

		const Container& Data() const { return m_Map; }
		const size_t GetHeigth() const { return Height; }
		const size_t GetWidth() const { return Width; }
		const size_t GetOffset() const { return Offset; }
		
		std::vector<bool>::reference At(Vector2 v) { return m_Map[v.X - Offset][v.Y]; }
		std::vector<bool>::reference AtChecked(Vector2 v)
		{
			while (v.X - Offset < 0)
			{
				AppendSide(true);
			}
			while (v.X - Offset >= m_Map.size())
			{
				AppendSide(false);
			}

			return m_Map[v.X - Offset][v.Y];
		}
	
		void AppendSide(bool isLeftOne)
		{
			std::vector<bool> newLine(Height, true);
			newLine[Height - 1] = false;

			if (isLeftOne)
			{
				Offset--;
				m_Map.insert(m_Map.begin(), newLine);
			}
			else 
			{
				m_Map.push_back(newLine);
			}

			Width++;
		}

private:

		int Offset;
		int Width;
		int Height;
		Container m_Map;
	};


	Map::Map(std::istream& str, bool isFirstPuzzle)
	{
		std::string buffer;
		std::vector<std::vector<Vector2>> lines;
		std::array<char, 6> charBuffer;
		Height = 0;
		Width = 0;
		Offset = std::numeric_limits<int>::max();

		while (!str.eof())
		{
			std::getline(str, buffer);
			std::vector<std::string> tokenized = tokenize(buffer, " -> ");
			std::vector<Vector2> line;

			for (std::string token : tokenized)
			{
				Vector2 v;

				size_t indexOfDel = token.find(',');
				std::memcpy(charBuffer.data(), (void*)token.c_str(), indexOfDel);
				charBuffer[indexOfDel] = '\0';
				v.X = atoi(charBuffer.data());

				std::memcpy(charBuffer.data(), (void*)(token.c_str() + indexOfDel + 1), token.size() - indexOfDel - 1);
				charBuffer[token.size() - indexOfDel - 1] = '\0';
				v.Y = atoi(charBuffer.data());

				line.push_back(v);

				Offset = std::min(Offset, v.X);
				Width = std::max(Width, v.X);
				Height = std::max(Height, v.Y);
			}

			lines.push_back(line);
		}

		Width -= Offset;
		Width++;
		Height++;
		if (!isFirstPuzzle)
		{
			Height += 2;
		}

		std::vector<bool> referenceLine(Height, true);
		if (!isFirstPuzzle)
		{
			referenceLine[Height - 1] = false;
		}

		m_Map.resize(Height, referenceLine);

		for (std::vector<Vector2> line : lines)
		{
			for (int i = 1; i < line.size(); i++)
			{
				Vector2 direction = line[i] - line[i - 1];
				direction.X = sign(direction.X);
				direction.Y = sign(direction.Y);

				Vector2 current = line[i - 1];

				At(current) = false;
				while (current != line[i])
				{
					current = current + direction;
					At(current) = false;
				}
			}
		}
	}

	bool Puzzle1Step(Map& m, Vector2 sandPos, size_t& counter)
	{
		if (sandPos.X != m.GetOffset())
		{
			Vector2 currentSandPos = sandPos;
			sandPos.Y++;
			if (m.At(sandPos))
			{
				if (Puzzle1Step(m, sandPos, counter))
				{
					return true;
				}
			}

			sandPos.X--;
			if (m.At(sandPos))
			{
				if (Puzzle1Step(m, sandPos, counter))
				{
					return true;
				}
			}

			sandPos.X += 2;
			if (m.At(sandPos))
			{
				if (Puzzle1Step(m, sandPos, counter))
				{
					return true;
				}
			}

			m.At(currentSandPos) = false;
			counter++;
			return false;
		}

		return true;
	}

	void Puzzle2Step(Map& m, Vector2 sandPos, size_t& counter)
	{
		Vector2 currentSandPos = sandPos;
		sandPos.Y++;
		if (m.At(sandPos))
		{
			Puzzle2Step(m, sandPos, counter);
		}

		sandPos.X--;
		if (m.AtChecked(sandPos))
		{
			Puzzle2Step(m, sandPos, counter);
		}

		sandPos.X += 2;
		if (m.AtChecked(sandPos))
		{
			Puzzle2Step(m, sandPos, counter);
		}

		m.At(currentSandPos) = false;
		counter++;
	}
}

void print(Day14::Map& m)
{
	std::cout << "*--------------------------------------------*" << std::endl;
	char chars[2]{ '#', '.' };
	for (int y = 0; y < m.GetHeigth(); y++)
	{
		for (int x = m.GetOffset(); x < m.GetOffset() + m.GetWidth(); x++)
		{
			std::cout << chars[m.At({ x, y })];
		}
		std::cout << std::endl;
	}

	std::cout << "*--------------------------------------------*" << std::endl;
}

std::string PuzzleApproach<14, 1>::RunTest(std::istream& stream)
{
	Day14::Map map(stream, true);
	size_t counter = 0;
	Day14::Puzzle1Step(map, Vector2(500, 0), counter);
	return std::to_string(counter);
}

std::string PuzzleApproach<14, 2>::RunTest(std::istream& stream)
{
	Day14::Map map(stream, false);
	size_t counter = 0;
	Day14::Puzzle2Step(map, Vector2(500, 0), counter);
	return std::to_string(counter);
}