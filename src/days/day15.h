#pragma once

#include "Common.h"
#include "Helpers.h"

#undef CURRENT_DAY
#define CURRENT_DAY 15

//template<>
//constexpr char RunPuzzleTraits<15>::Value = RunPuzzleTraitsConstant::RunTest;

namespace Day15
{
	int parse(std::string& str, size_t beginIndex, size_t endIndex)
	{
		std::array<char, 10> buffer;
		std::memcpy(buffer.data(), str.c_str() + beginIndex, endIndex - beginIndex);
		buffer[endIndex - beginIndex] = '\0';
		return atoi(buffer.data());
	}

	int parseNext(std::string& str, size_t& offset, char firstSearch, char secondSearch)
	{
		size_t beginIndex = str.find(firstSearch, offset);
		offset = str.find(secondSearch, beginIndex);
		return parse(str, beginIndex + 1, offset);
	}

	int parseEnd(std::string& str, size_t& offset, char firstSearch)
	{
		size_t beginIndex = str.find(firstSearch, offset);
		offset = str.size();
		return parse(str, beginIndex + 1, offset);
	}

	int Manhattan(Vector2 lhv, Vector2 rhv)
	{
		return std::abs(lhv.X - rhv.X) + std::abs(lhv.Y - rhv.Y);
	}

	struct Range
	{
		int Min;
		int Max;
		bool Unused;
	};

	struct Measurement
	{
		Vector2 Sensor;
		Vector2 Beacon;
		int CachedRange;
	};

	bool TestPuzzle2(std::vector<Measurement>& input, Vector2 pos, int maxLine)
	{
		//std::cout << "Test pos " << std::to_string(pos) << std::endl;

		if (pos.X < 0 || pos.X > maxLine) { return false; }
		if (pos.Y < 0 || pos.Y > maxLine) { return false; }

		for (auto& measurement : input)
		{
			if (measurement.Beacon == pos)
			{
				return false;
			}

			if (Day15::Manhattan(measurement.Sensor, pos) <= measurement.CachedRange)
			{
				return false;
			}
		}

		return true;
	}

	std::string evaluateResult(Vector2 v)
	{
		long unsigned int head = v.X * 4 + v.Y / 1000000;
		long unsigned int tail = v.Y % 1000000;
		char buffer[21];
		snprintf(buffer, 21, "%i%06i", head, tail);
		return buffer;
	}
}



std::string PuzzleApproach<15, 1>::RunTest(std::istream& stream)
{
	std::vector<std::pair<Vector2, Vector2>> input;
	std::string line;
	std::getline(stream, line);
	int queryLine = stoi(line);

	while (!stream.eof())
	{
		std::getline(stream, line);

		std::pair<Vector2, Vector2> parseResult;
		size_t offset = 0;

		parseResult.first.X = Day15::parseNext(line, offset, '=', ',');
		parseResult.first.Y = Day15::parseNext(line, offset, '=', ':');

		parseResult.second.X = Day15::parseNext(line, offset, '=', ',');
		parseResult.second.Y = Day15::parseEnd(line, offset, '=');

		input.push_back(parseResult);
	}

	std::vector<Day15::Range> Ranges;
	for (auto measurement : input)
	{
		const int range = Day15::Manhattan(measurement.second, measurement.first);
		const Vector2 closestOnQueryLine(measurement.first.X, queryLine);
		const int rangeToQueryLine = Day15::Manhattan(measurement.first, closestOnQueryLine);
		const int rangeDelta = range - rangeToQueryLine;

		if (rangeDelta >= 0)
		{
			Day15::Range r;
			r.Unused = false;
			r.Min = closestOnQueryLine.X; r.Min -= rangeDelta; 
			r.Max = closestOnQueryLine.X; r.Max += rangeDelta;
			if (measurement.second.Y == queryLine)
			{
				if (measurement.second.X == r.Max)
				{
					r.Max--;
					if (r.Max < r.Min) { continue; }
				}
				else
				{
					r.Min++;
					if (r.Max < r.Min) { continue; }
				}
			}
			Ranges.push_back(r);
		}
	}

	std::sort(Ranges.begin(), Ranges.end(), [](const Day15::Range& lhv, const Day15::Range rhv)
		{
			return lhv.Min < rhv.Min;
		});

	int activeIndex = 0;
	for (int i = 1; i < Ranges.size(); i++)
	{
		if (Ranges[activeIndex].Max >= Ranges[i].Min)
		{
			Ranges[activeIndex].Max = std::max(Ranges[i].Max, Ranges[activeIndex].Max);
			Ranges[i].Unused = true;
		}
		else 
		{
			activeIndex = i;
		}
	}

	size_t counter = 0;
	for (int i = 0; i < Ranges.size(); i++)
	{
		if (!Ranges[i].Unused)
		{
			counter += Ranges[i].Max - Ranges[i].Min + 1;
		}
	}

	return std::to_string(counter);
}

std::string PuzzleApproach<15, 2>::RunTest(std::istream& stream)
{
	std::vector<Day15::Measurement> input;
	std::string line;
	std::getline(stream, line);
	int queryLine = stoi(line) * 2;

	while (!stream.eof())
	{
		std::getline(stream, line);

		Day15::Measurement measurement;
		size_t offset = 0;

		measurement.Sensor.X = Day15::parseNext(line, offset, '=', ',');
		measurement.Sensor.Y = Day15::parseNext(line, offset, '=', ':');

		measurement.Beacon.X = Day15::parseNext(line, offset, '=', ',');
		measurement.Beacon.Y = Day15::parseEnd(line, offset, '=');

		measurement.CachedRange = Day15::Manhattan(measurement.Beacon, measurement.Sensor);

		input.push_back(measurement);
	}

	for (auto measurement : input)
	{
		for (int value = 0; value < measurement.CachedRange + 1; value++)
		{
			int xOffset = value;
			int yOffset = measurement.CachedRange - xOffset + 1;
			Vector2 pos = measurement.Sensor + Vector2(xOffset, yOffset);
			if (Day15::TestPuzzle2(input, pos, queryLine)) { return Day15::evaluateResult(pos); }
			pos.Y -= yOffset + yOffset;
			if (Day15::TestPuzzle2(input, pos, queryLine)) { return Day15::evaluateResult(pos); }
			pos.X -= xOffset + xOffset;
			if (Day15::TestPuzzle2(input, pos, queryLine)) { return Day15::evaluateResult(pos); }
			pos.Y += yOffset + yOffset;
			if (Day15::TestPuzzle2(input, pos, queryLine)) { return Day15::evaluateResult(pos); }
		}
	}

	INVALID_INPUT;
}
