#pragma once

#include "Common.h"
#include "Helpers.h"

#undef CURRENT_DAY
#define CURRENT_DAY 17

//template<>
//constexpr char RunPuzzleTraits<17>::Value = RunPuzzleTraitsConstant::RunTest;

namespace Day17
{
	template <typename T>
	constexpr inline T oned() { return (T)-1; }

	struct Map
	{
		Map()
			: CapacityInRows(1)
			, FloorRow(1)
		{
			Bitmask = (uint16_t*)calloc(1, sizeof(uint16_t));
			assert(Bitmask);
			*Bitmask = oned<uint16_t>();
		}

		uint16_t* GetBitmask(size_t fromRow) 
		{
			assert(fromRow + 3 < CapacityInRows);
			return Bitmask + fromRow;
		}

		// safety check for type overflowing required
		void EnsureCapacity(size_t rows)
		{
			if (CapacityInRows < rows)
			{
				unsigned long nextCapacityT;
				_BitScanReverse64(&nextCapacityT, rows);
				size_t nextCapacity = 1LL << (1 + nextCapacityT);
				auto realloced = (uint16_t*)realloc(Bitmask, nextCapacity * sizeof(uint16_t)); // (capacity * 2)
				assert(realloced);
				Bitmask = realloced;
				for (size_t i = CapacityInRows; i < nextCapacity; i++)
				{
					constexpr uint16_t initialValue = (oned<uint16_t>() << (RightOffset + CaveWidth)) | (oned<uint16_t>() >> (LeftOffset + CaveWidth));
					*(Bitmask + i) = initialValue;
				}
				CapacityInRows = nextCapacity;
			}
		}

		size_t FloorRow;

	private:

		static constexpr size_t Width = sizeof(uint16_t) * 8;
		static constexpr size_t CaveWidth = 7;
		static constexpr size_t LeftOffset = 1;
		static constexpr size_t RightOffset = Width - CaveWidth - LeftOffset;

		size_t CapacityInRows;
		uint16_t* Bitmask;
	};

	constexpr size_t shapesCount = 5;
	uint64_t nextShape(int& lastShape)
	{
		// 16 bit per line, start from left-down corner
		static uint64_t shapes[shapesCount]
		{
			0b1111LL << 9,
			(1LL << 11) | (0b111LL << (10 + 16)) | (1LL << (11 + 32)),
			(0b111LL << 10) | (1LL << (10 + 16)) | (1LL << (10 + 32)),
			(1LL << 12) | (1LL << (12 + 16)) | (1LL << (12 + 32)) | (1LL << (12 + 48)),
			(0b11LL << 11) | (0b11LL << (11 + 16))
		};

		lastShape = ++lastShape == shapesCount ? 0 : lastShape;
		return shapes[lastShape];
	}

	struct PeriodHash
	{
	public:
		PeriodHash() : m_Hash(0) { }
		uint64_t GetHash() const { return m_Hash; }
		void Clear() { m_Hash = 0; }
		void Append(uint64_t newHash)
		{
			m_Hash ^= newHash + 0x9e3779b9 + (m_Hash << 6) + (m_Hash >> 2);
		}

	private:
		uint64_t m_Hash;
	};

	std::optional<size_t> findPeriod(const std::vector<std::tuple<uint64_t, size_t>>& map, const uint64_t newHash)
	{
		size_t periodStart;
		size_t half = map.size() / 2 - 1;

		for (int i = (int)map.size() - 1; i > half; i--)
		{
			if (std::get<0>(map[i]) == newHash)
			{
				periodStart = i;
				size_t delta = map.size() - periodStart;
				bool success = true;
				for (size_t j = map.size() - 1; j >= periodStart; --j)
				{
					if (std::get<0>(map[j]) != std::get<0>(map[j - delta]))
					{
						success = false;
						break;
					}
				}

				if (success)
				{
					return i;
				}
			}
		}

		return std::optional<size_t>();
	}

	inline uint64_t step(uint32_t& patternIndex, const std::string& pattern, Map& map, int& shapeIndex)
	{
		size_t row = 3 + map.FloorRow;
		map.EnsureCapacity(5 + row);
		uint64_t shape = Day17::nextShape(shapeIndex);

		while (true)
		{
			patternIndex = ++patternIndex == pattern.size() ? 0 : patternIndex;
			char shifting = pattern[patternIndex];

			uint64_t shiftedShape = shifting == '<' ? shape << 1 : shape >> 1;
			uint64_t& wall = *reinterpret_cast<uint64_t*>(map.GetBitmask(row));
			if (!(shiftedShape & wall))
			{
				shape = shiftedShape;
			}

			uint64_t downwall = *reinterpret_cast<uint64_t*>(map.GetBitmask(--row));
			if (shape & downwall)
			{
				wall = wall | shape;
				break;
			}
		}

		unsigned long index;
		_BitScanReverse64(&index, shape);
		map.FloorRow = std::max(map.FloorRow, row + 1 + (index >> 4) + 1);

		return shape;
	}

	std::string solve(std::istream& stream, const size_t requiredRocks)
	{
		Day17::Map map;

		std::string pattern;
		std::getline(stream, pattern);

		size_t rocksFallen = 0;
		int shapeIndex = -1;
		unsigned int patternIndex = -1;
		PeriodHash hash;
		std::vector<std::tuple<uint64_t, size_t>> cachedPeriods{ { 0, 1 } };

		do
		{
			uint64_t shape = step(patternIndex, pattern, map, shapeIndex);
			hash.Append(shape);

			if (shapeIndex == 0)
			{
				auto periodIndex = findPeriod(cachedPeriods, hash.GetHash());
				if (periodIndex)
				{
					size_t cachedFloorRow = std::get<1>(cachedPeriods[periodIndex.value()]);
					size_t deltaRowPerPeriod = map.FloorRow - cachedFloorRow;

					size_t stepsToSimulate = (requiredRocks - (rocksFallen + 1)) % shapesCount;
					size_t periodsLeft = (requiredRocks - (rocksFallen + stepsToSimulate + 1)) / shapesCount;
					size_t periodsMultiplier = cachedPeriods.size() - periodIndex.value();

					size_t simulateStepsLeft = stepsToSimulate + shapesCount * (periodsLeft % periodsMultiplier);
					periodsLeft /= periodsMultiplier;

					while (simulateStepsLeft-- > 0)
					{
						step(patternIndex, pattern, map, shapeIndex);
					}

					map.FloorRow += deltaRowPerPeriod * periodsLeft;
					rocksFallen = requiredRocks - 1;
				}
				else
				{
					cachedPeriods.emplace_back(hash.GetHash(), map.FloorRow);
					hash.Clear();
				}
			}
		} while (++rocksFallen < requiredRocks);

		return std::to_string(map.FloorRow - 1);
	}
}

std::string PuzzleApproach<17, 1>::RunTest(std::istream& stream)
{
	return Day17::solve(stream, 2022);
}

std::string PuzzleApproach<17, 2>::RunTest(std::istream& stream)
{
	return Day17::solve(stream, 1000000000000);
}
