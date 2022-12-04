#include "Common.h"

#include <iostream>
#include <fstream>

#define SEE_ALL_DAYS_RESULT 0
#define CURRENT_DAY 4

template<int day>
struct NeedShowDayResults
{
#if SEE_ALL_DAYS_RESULT
	constexpr static bool Value = true;
#else
	constexpr static bool Value = false;
#endif
};

template<>
constexpr bool NeedShowDayResults<CURRENT_DAY>::Value = true;

template<int day, int puzzle>
void Run()
{
	char filename[13];
	snprintf(filename, 13, "input/%02i.txt", day);

	std::ifstream file(filename);

	std::string result = PuzzleApproach<day, puzzle>().RunTest(file);
	std::cout << "Test [" << day << ";" << puzzle << "] answer: " << result << std::endl;
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
		Run<day, 1>();
		Run<day, 2>();
		Separator();
	}
}

int main()
{
	RunDay<1>();
	RunDay<2>();
	RunDay<3>();
	RunDay<4>();

	std::cin.get();
}