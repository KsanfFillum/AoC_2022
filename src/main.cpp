#define SEE_ALL_DAYS_RESULT 0

#include "Common.h"
#include "Profiler.h"

#include <iostream>
#include <fstream>

#ifndef CURRENT_DAY
#define CURRENT_DAY 1
#endif // !CURRENT_DAY

template<>
constexpr bool NeedShowDayResults<CURRENT_DAY>::Value = true;

template<char puzzleBit>
void FormatFilename(char* buffer, int day);

template<>
void FormatFilename<RunPuzzleTraitsConstant::RunTest>(char* buffer, int day)
{
	snprintf(buffer, 18, "input/%02i_test.txt", day);
}

template<>
void FormatFilename<RunPuzzleTraitsConstant::RunMain>(char* buffer, int day)
{
	snprintf(buffer, 18, "input/%02i.txt", day);
}

template<int day, int puzzle, char puzzleBit>
constexpr std::string GetDescription()
{
	std::string s = puzzleBit == RunPuzzleTraitsConstant::RunMain ? "Puzzle" : "Test  ";
	s += " [";
	s += day < 10 ? " " : "";
	s += std::to_string(day);
	s += "; ";
	s += std::to_string(puzzle);
	s += "]";

	return s;
}

Profiler GlobalProfiler;
template<int day, int puzzle, char puzzleBit>
void Run()
{
	if (RunPuzzleTraits<day>::Value & puzzleBit)
	{
		char filename[18];
		
		FormatFilename<puzzleBit>(filename, day);

		std::ifstream file(filename);
		assert(file.good());

		GlobalProfiler.Start(GetDescription<day, puzzle, puzzleBit>());
		std::string result = PuzzleApproach<day, puzzle>().RunTest(file);
		GlobalProfiler.End();

		std::cout << GetDescription<day, puzzle, puzzleBit>() << " answer: " << result << std::endl;
	}
}

void Separator()
{
	std::cout << "*------------------------------------------*" << std::endl;
}

template<int day>
void RunDay()
{
	if (NeedShowDayResults<day>::Value)
	{
		Run<day, 1, RunPuzzleTraitsConstant::RunTest>();
		Run<day, 1, RunPuzzleTraitsConstant::RunMain>();

		Run<day, 2, RunPuzzleTraitsConstant::RunTest>();
		Run<day, 2, RunPuzzleTraitsConstant::RunMain>();

		Separator();
	}
}

template<int lastDay>
void RunAllDays()
{
	RunAllDays<lastDay - 1>();
	RunDay<lastDay>();
}

template<>
void RunAllDays<1>()
{
	RunDay<1>();
}

int main()
{
	RunAllDays<CURRENT_DAY>();
	std::cout << std::endl;
	GlobalProfiler.Flush();

	std::cin.get();
}