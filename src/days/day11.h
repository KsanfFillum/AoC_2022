#pragma once

#include "Common.h"
#include "Helpers.h"

namespace Day11
{
	using uint64 = unsigned long long;
	using Operator = std::function<uint64(uint64, uint64)>;
	
	std::array<Operator, 4> Operators
	{
		[](uint64 val, uint64 arg) { return val + arg; },
		[](uint64 val, uint64 arg) { return val * arg; },
		[](uint64 val, uint64 arg) { return val + val; },
		[](uint64 val, uint64 arg) { return val * val; }
	};

	int IndexOfOperator(bool isMultiply, bool isSelf) { return (int)isMultiply | (isSelf << 1); }

	int FindLastNumber(std::string str)
	{
		const int lastDelimeter = (int)str.find_last_of(' ');
		return atoi(str.c_str() + lastDelimeter);
	}

	struct Monkey
	{
		Monkey(std::istream& stream)
		{
			std::array<std::string, 6> lines;
			for (int i = 0; i < 6; i++)
			{
				std::getline(stream, lines[i]);
			}

			auto tokens = tokenize(lines[1], ' ');
			for (int i = 2; i < tokens.size(); i++)
			{
				Items.push(stoi(tokens[i]));
			}

			tokens = tokenize(lines[2], ' ');
			bool argumentIsSelf = tokens[5] == "old";
			bool isMultiply = tokens[4] == "*";
			Op = Operators[IndexOfOperator(isMultiply, argumentIsSelf)];
			if (!argumentIsSelf)
			{
				OperationConstant = stoi(tokens[5]);
			}
				
			TestConstant = FindLastNumber(lines[3]);
			MonkeyOnSuccess = FindLastNumber(lines[4]);
			MonkeyOnFail = FindLastNumber(lines[5]);
		}

		int TestConstant;
		int OperationConstant;
		int MonkeyOnSuccess;
		int MonkeyOnFail;
		Operator Op;

		std::queue<uint64> Items;
	};

	template<int puzzle, int rounds>
	std::string Approach(std::istream& stream)
	{
		std::vector<Day11::Monkey> monkeys;
		std::vector<int> processedItems;
		uint64 divider = 1;
		while (!stream.eof())
		{
			monkeys.emplace_back(stream);
			std::string buffer;
			std::getline(stream, buffer);
			divider *= monkeys.back().TestConstant;
		}

		processedItems.resize(monkeys.size(), 0);

		for (int i = 0; i < rounds; i++)
		{
			for (int m = 0; m < monkeys.size(); m++)
			{
				Day11::Monkey& monkey = monkeys[m];
				processedItems[m] += (int)monkey.Items.size();
				while (monkey.Items.size() > 0)
				{
					uint64 item = monkey.Items.front(); monkey.Items.pop();
					item = monkey.Op(item, monkey.OperationConstant);
					if (puzzle == 1)
					{
						item /= 3;
					}
					else
					{
						item = item % divider;
					}

					int throwTarget = (item % monkey.TestConstant == 0) ? monkey.MonkeyOnSuccess : monkey.MonkeyOnFail;
					monkeys[throwTarget].Items.push(item);
				}
			}
		}

		uint64 best = 0;
		uint64 best2 = 0;

		for (auto count : processedItems)
		{
			if (count > best)
			{
				best2 = best;
				best = count;
			}
			else if (count > best2)
			{
				best2 = count;
			}
		}

		return std::to_string(best * best2);
	}
}

std::string PuzzleApproach<11, 1>::RunTest(std::istream& stream)
{
	return Day11::Approach<1, 20>(stream);
}

std::string PuzzleApproach<11, 2>::RunTest(std::istream& stream)
{
	return Day11::Approach<2, 10000>(stream);
}