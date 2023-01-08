#pragma once

#include <chrono>

class Profiler
{
	using TimeStamp = std::chrono::system_clock::time_point;

public:
	void Start(std::string name) 
	{
		CurrentName = name;
		BeginPoint = std::chrono::system_clock::now();
	}

	void End()
	{
		auto endPoint = std::chrono::system_clock::now();
		assert(BeginPoint != TimeStamp());
		Points.emplace_back(CurrentName, endPoint - BeginPoint);

		BeginPoint = TimeStamp();
	}

	void Flush()
	{
		BeginPoint = TimeStamp();
		std::chrono::system_clock::duration total = BeginPoint - BeginPoint;
		for (auto point : Points)
		{
			total += point.second;
		}

		for (int i = 0; i < Points.size(); i++)
		{
			constexpr char format[]{ "%s: %8lli mks (%2.2f%%)\n" };
			char buffer[64];
			snprintf(buffer, 64, format
				, Points[i].first.c_str()
				, std::chrono::duration_cast<std::chrono::microseconds>(Points[i].second).count()
				, ((float)Points[i].second.count() / total.count()) * 100);
			std::cout << buffer;
		}
	}

private:
	
	TimeStamp BeginPoint;
	std::string CurrentName;
	std::vector<std::pair<std::string, std::chrono::system_clock::duration>> Points;
};