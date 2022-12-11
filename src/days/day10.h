#pragma once

#include "Common.h"
#include "Helpers.h"

namespace Day10
{
	enum class FunctionResult
	{
		InProcess,
		Done
	};

	struct Function
	{
		virtual void Init(std::string line) = 0;
		virtual FunctionResult Tick(int& deviceRegister) = 0;
	};

	struct Addx : public Function
	{
		virtual void Init(std::string line)
		{
			assert(isFirstTick);
			additor = atoi(line.c_str() + 5);
		}

		virtual FunctionResult Tick(int& deviceRegister) override
		{
			ScopeSetter<bool> s(isFirstTick, !isFirstTick);
			if (isFirstTick)
			{
				return FunctionResult::InProcess;
			}
			else
			{
				deviceRegister += additor;
				return FunctionResult::Done;
			}
		}

	private:
		bool isFirstTick = true;
		int additor;
	};

	struct Noop : public Function
	{
		virtual void Init(std::string line) override { }
		virtual FunctionResult Tick(int& deviceRegister) override { return FunctionResult::Done; }
	};

	std::string Draw(std::array<char, 240> screenBuffer)
	{
		std::stringstream sstream;
		sstream << std::endl;
		for (int i = 0; i < 6; i++)
		{
			sstream << std::string(screenBuffer.begin() + (i * 40), screenBuffer.begin() + ((i + 1) * 40)) << std::endl;
		}

		return sstream.str();
	}
	
	std::map<std::string, Function*> Functors
	{
		{ "addx", new Addx() },
		{ "noop", new Noop() }
	};
}

std::string PuzzleApproach<10, 1>::RunTest(std::istream& stream)
{
	std::stack<int> interestingTacts({ 220, 180, 140, 100, 60, 20 });
	int resultCounter = 0;
	int deviceRegister = 1;
	int cycle = 0;

	while (!stream.eof() && interestingTacts.size() > 0)
	{
		std::string buffer;
		std::getline(stream, buffer);

		Day10::Function* functor = Day10::Functors[buffer.substr(0, 4)];
		functor->Init(buffer);

		Day10::FunctionResult outResult;
		do
		{
			if (++cycle == interestingTacts.top())
			{
				resultCounter += interestingTacts.top() * deviceRegister;
				interestingTacts.pop();
			}

			outResult = functor->Tick(deviceRegister);
		} while (outResult == Day10::FunctionResult::InProcess);
	}

	return std::to_string(resultCounter);
}


std::string PuzzleApproach<10, 2>::RunTest(std::istream& stream)
{
	int deviceRegister = 1;
	int cycle = 0;
	std::array<char, 240> screenBuffer;

	while (!stream.eof())
	{
		std::string buffer;
		std::getline(stream, buffer);

		Day10::Function* functor = Day10::Functors[buffer.substr(0, 4)];
		functor->Init(buffer);

		Day10::FunctionResult outResult;
		do
		{
			screenBuffer[cycle] = std::abs(deviceRegister - (cycle % 40)) <= 1 ? '#' : '.';
			outResult = functor->Tick(deviceRegister);
			cycle++;
		} while (outResult == Day10::FunctionResult::InProcess);
	}

	return Day10::Draw(screenBuffer);
}
