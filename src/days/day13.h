#pragma once

#include "Common.h"

namespace Day13
{
	template<typename T>
	struct Optional
	{
		Optional()
			: Value(), HasValue(false)
		{
		}

		Optional(const T& value)
			: Value(value), HasValue(true)
		{
		}

		T Value;
		bool HasValue;
	};
	
	struct Value
	{
		int Integer;
		std::vector<Value> List;

		bool IsInteger;
	};

	Value ParseInternal(std::string string)
	{
		std::array<Value, 16> valueStack; size_t stackSize = 1; valueStack[1].IsInteger = false;
		std::array<char, 16> buffer; int bufferSize = 0;

		char prevChar = '[';
		for (char ch : string)
		{
			if (ch == '[')
			{
				valueStack[++stackSize].IsInteger = false;
			}
			else if (ch == ']')
			{
				if (bufferSize > 0)
				{
					buffer[bufferSize++] = '\0';
					bufferSize = 0;

					valueStack[stackSize].IsInteger = true;
					valueStack[stackSize].Integer = atoi(buffer.data());
				}

				valueStack[stackSize - 1].List.push_back(valueStack[stackSize]);
				valueStack[stackSize--].List.clear();
			}
			else if (ch == ',')
			{
				if (bufferSize > 0)
				{
					buffer[bufferSize++] = '\0';
					bufferSize = 0;

					valueStack[stackSize].IsInteger = true;
					valueStack[stackSize].Integer = atoi(buffer.data());
				}

				valueStack[stackSize - 1].List.push_back(valueStack[stackSize]);
				valueStack[stackSize].List.clear();
			}
			else
			{
				buffer[bufferSize++] = ch;
			}

			prevChar = ch;
		}

		return valueStack[1];
	}

	std::vector<Value> Parse(std::istream& stream)
	{
		std::vector<Value> parsed;
		std::string buffer;
		while (!stream.eof())
		{
			std::getline(stream, buffer); parsed.push_back(ParseInternal(buffer));
			std::getline(stream, buffer); parsed.push_back(ParseInternal(buffer));
			std::getline(stream, buffer); 
		}

		return parsed;
	}

	int Compare(const Value& lhv, const Value& rhv)
	{
		if (lhv.IsInteger == rhv.IsInteger)
		{
			if (lhv.IsInteger)
			{
				return lhv.Integer - rhv.Integer;
			}
			else
			{
				for (size_t i = 0; ; i++)
				{
					if (lhv.List.size() <= i)
					{
						if (rhv.List.size() <= i)
						{
							return 0;
						}
						else
						{
							return -1;
						}
					}

					if (rhv.List.size() <= i)
					{
						return 1;
					}

					int compareRes = Compare(lhv.List[i], rhv.List[i]);
					if (compareRes != 0)
					{
						return compareRes;
					}
				}
			}
		}
		else
		{
			const Value* t_lhv = &lhv;
			const Value* t_rhv = &rhv;

			Value tValue;
			tValue.IsInteger = false;
			tValue.List.clear();

			if (lhv.IsInteger)
			{
				tValue.List.push_back(lhv);
				t_lhv = &tValue;
			}
			else
			{
				tValue.List.push_back(rhv);
				t_rhv = &tValue;
			}

			int comparant = Compare(*t_lhv, *t_rhv);
			if (comparant == 0)
			{
				return lhv.IsInteger ? -1 : 1;
			}

			return comparant;
		}

		assert(false);
	}

	bool operator ==(const Value& lhv, const Value rhv)
	{
		return lhv.IsInteger == rhv.IsInteger && lhv.IsInteger ? (rhv.Integer == lhv.Integer) : (rhv.List == lhv.List);
	}
}

std::string PuzzleApproach<13, 1>::RunTest(std::istream& stream)
{
	auto pairs = Day13::Parse(stream);
	size_t sum = 0;
	for (int i = 0; i < pairs.size(); i += 2)
	{
		if (Day13::Compare(pairs[i], pairs[i + 1]) < 0)
		{
			sum += 1 + (i / 2);
		}
	}

	return std::to_string(sum);
}

std::string PuzzleApproach<13, 2>::RunTest(std::istream& stream)
{
	auto pairs = Day13::Parse(stream);
	std::stringstream sstream;
	sstream << "[[2]]\n[[6]]";
	auto reqValues = Day13::Parse(sstream);

	pairs.insert(pairs.end(), reqValues.begin(), reqValues.end());

	size_t sum = 0;

	std::sort(pairs.begin(), pairs.end(), [](const Day13::Value& lhv, const Day13::Value& rhv)
		{
			return Day13::Compare(lhv, rhv) < 0;
		});

	size_t result = 1;
	for (int i = 0; i < pairs.size(); i++)
	{
		const Day13::Value& testItem = pairs[i];
		if (testItem.List.size() == 1 && testItem.List[0].List.size() == 1 && testItem.List[0].List[0].IsInteger)
		{
			if (testItem.List == reqValues[0].List || testItem.List == reqValues[1].List)
			{
				result *= i + 1;
			}
		}
	}

	return std::to_string(result);
}