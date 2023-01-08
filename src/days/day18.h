#pragma once

#include "Common.h"
#include "Helpers.h"
#include <unordered_set>

#undef CURRENT_DAY
#define CURRENT_DAY 18

//template<>
//constexpr char RunPuzzleTraits<18>::Value = RunPuzzleTraitsConstant::RunTest;

namespace Day18
{
	// axis bounds are [0; 30) (with two sentinel values 0 and 31)
	constexpr int ShiftingSize = 5;

	constexpr int XShifting = ShiftingSize * 0;
	constexpr int YShifting = ShiftingSize * 1;
	constexpr int ZShifting = ShiftingSize * 2;

	constexpr int xRestore(uint16_t hash) { return (hash >> XShifting) & ((1 << ShiftingSize) - 1); }
	constexpr int yRestore(uint16_t hash) { return (hash >> YShifting) & ((1 << ShiftingSize) - 1); }
	constexpr int zRestore(uint16_t hash) { return (hash >> ZShifting) & ((1 << ShiftingSize) - 1); }

	inline uint16_t vectorHash(const Vector3& v)
	{
		return v.X << XShifting | v.Y << YShifting | v.Z << ZShifting;
	}

	Vector3 restoreVector(uint16_t hash)
	{
		return Vector3(xRestore(hash), yRestore(hash), zRestore(hash));
	}

	std::set<uint16_t> parseInput(std::istream& stream)
	{
		std::set<uint16_t> points;

		std::string line;
		int coordBuffer[3];

		while (!stream.eof())
		{
			std::getline(stream, line);
			std::vector<std::string> coords = tokenize(line, ',');

			for (int i = 0; i < 3; i++)
			{
				coordBuffer[i] = stoi(coords[i]);
			}

			const Vector3 vector(coordBuffer[0], coordBuffer[1], coordBuffer[2]);
			points.emplace(vectorHash(vector));
		}

		return points;
	}

	void processPoint(int& counter, std::set<uint16_t>& unprocessedPoints, std::unordered_set<uint16_t>& processedPoints, uint16_t current)
	{
		if (unprocessedPoints.count(current) == 0)
		{
			counter += 1 - processedPoints.count(current);
			return;
		}

		unprocessedPoints.erase(current);
		processedPoints.insert(current);

		for (int i = 0; i < 3; i++)
		{
			const uint16_t offset = (1 << (i * ShiftingSize));
			processPoint(counter, unprocessedPoints, processedPoints, current + offset);
			processPoint(counter, unprocessedPoints, processedPoints, current - offset);
		}
	}

	bool validatePocket(std::set<uint16_t>& unprocessedPoints, std::unordered_set<uint16_t>& processedPoints, const std::set<uint16_t>& points, uint16_t current)
	{
		if (points.count(current) | processedPoints.count(current)) 
		{
			return true;
		}

		if (!unprocessedPoints.count(current))
		{
			return false;
		}

		unprocessedPoints.erase(current);
		processedPoints.insert(current);

		for (int i = 0; i < 3; i++)
		{
			const uint16_t offset = (1 << (i * ShiftingSize));
			if (!validatePocket(unprocessedPoints, processedPoints, points, current + offset)
				|| !validatePocket(unprocessedPoints, processedPoints, points, current - offset))
			{
				return false;
			}
		}

		return true;
	}

	template<typename TCont, typename TPred>
	void mapRemoveIf(TCont& cont, const TPred pred)
	{
		for (auto it = cont.begin(); it != cont.end();)
		{
			if (pred(*it)) { it = cont.erase(it); }
			else { ++it; }
		}
	}
}

std::string PuzzleApproach<18, 1>::RunTest(std::istream& stream)
{
	std::set<uint16_t> points = Day18::parseInput(stream);
	std::unordered_set<uint16_t> processedPoints;

	int counter = 0;
	while (points.size() > 0)
	{
		uint16_t point = *points.begin();
		Day18::processPoint(counter, points, processedPoints, point);
		processedPoints.clear();
	}
	
	return std::to_string(counter);
}

std::string PuzzleApproach<18, 2>::RunTest(std::istream& stream)
{
	std::set<uint16_t> points = Day18::parseInput(stream);
	std::set<uint16_t> unprocessedPoints = points;
	std::unordered_set<uint16_t> processedPoints;
	std::map<uint16_t, size_t> repeateXCounter;
	std::map<uint16_t, size_t> repeateYCounter;
	std::map<uint16_t, size_t> repeateZCounter;

	int counter = 0;
	while (unprocessedPoints.size() > 0)
	{
		uint16_t point = *unprocessedPoints.begin();
		Day18::processPoint(counter, unprocessedPoints, processedPoints, point);

		for (auto point : processedPoints)
		{
			const Vector3 v(Day18::xRestore(point), Day18::yRestore(point), Day18::zRestore(point));
			repeateXCounter[v.X]++;
			repeateYCounter[v.Y]++;
			repeateZCounter[v.Z]++;
		}

		processedPoints.clear();
	}

	Day18::mapRemoveIf(repeateXCounter, [](auto pair) { return pair.second < 2; });
	Day18::mapRemoveIf(repeateYCounter, [](auto pair) { return pair.second < 2; });
	Day18::mapRemoveIf(repeateZCounter, [](auto pair) { return pair.second < 2; });

	for (auto xPair : repeateXCounter)
	{
		for (auto yPair : repeateYCounter)
		{
			for (auto zPair : repeateZCounter)
			{
				uint16_t pocketHash = Day18::vectorHash(Vector3(xPair.first, yPair.first, zPair.first));
				if (!points.count(pocketHash))
				{
					unprocessedPoints.insert(pocketHash);
				}
			}
		}
	}

	while (unprocessedPoints.size() > 0)
	{
		uint16_t point = *unprocessedPoints.begin();
		if (Day18::validatePocket(unprocessedPoints, processedPoints, points, point))
		{
			for (auto hash : processedPoints)
			{
				for (int i = 0; i < 3; i++)
				{
					const uint16_t offset = (1 << (i * Day18::ShiftingSize));
					counter -= points.count(hash + offset);
					counter -= points.count(hash - offset);
				}
			}
		}

		processedPoints.clear();
	}

	return std::to_string(counter);
}