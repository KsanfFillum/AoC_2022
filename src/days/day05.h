#pragma once

#include "Common.h"
#include <stack>
#include <sstream>
#include <vector>

template<typename ContT>
inline void Add(ContT& cont, char ch);

template<>
inline void Add(std::stack<char>& cont, char ch)
{
	cont.push(ch);
}

template<>
inline void Add(std::vector<char>& cont, char ch)
{
	cont.push_back(ch);
}

template<typename ContT>
inline std::vector<ContT> FillCrates(std::istream& stream)
{
	std::vector<ContT> crates;
	std::stack<std::string> crateLines;

	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);
		if (line.empty())
		{
			crateLines.pop();
			break;
		}

		crateLines.push(line);
	}

	const size_t stacksCount = (crateLines.top().size() + 1) / 4;
	crates.resize(stacksCount);

	while (!crateLines.empty())
	{
		std::string line = crateLines.top();
		crateLines.pop();
		for (int i = 0; i < stacksCount; i++)
		{
			if (line[(i * 4) + 1] != ' ')
			{
				Add<ContT>(crates[i], line[(i * 4) + 1]);
			}
		}
	}

	return crates;
}

inline void FillIndicies(int& count, int& src, int& dst, std::istream& stream)
{
	std::string buffer;
	std::getline(stream, buffer, ' ');	// move
	std::getline(stream, buffer, ' ');	// move count
	count = stoi(buffer);
	std::getline(stream, buffer, ' ');	// from
	std::getline(stream, buffer, ' ');	// from index
	src = stoi(buffer) - 1;
	std::getline(stream, buffer, ' ');	// to
	std::getline(stream, buffer);		// dst index
	dst = stoi(buffer) - 1;
}

template<>
std::string PuzzleApproach<5, 1>::RunTest(std::istream& stream)
{
	std::vector<std::stack<char>> crates = FillCrates<std::stack<char>>(stream);

	while (!stream.eof())
	{
		int count;
		int src;
		int dst;

		FillIndicies(count, src, dst, stream);

		while (count-- > 0)
		{
			crates[dst].push(crates[src].top());
			crates[src].pop();
		}
	}

	std::stringstream ss;
	for (auto crate : crates)
	{
		ss << crate.top();
	}

	return ss.str();
}

template<>
std::string PuzzleApproach<5, 2>::RunTest(std::istream& stream)
{
	std::vector<std::vector<char>> crates = FillCrates<std::vector<char>>(stream);
	
	while (!stream.eof())
	{
		int count;
		int src;
		int dst;

		FillIndicies(count, src, dst, stream);

		crates[dst].insert(crates[dst].end(), crates[src].end() - count, crates[src].end());
		crates[src].resize(crates[src].size() - count);
	}

	std::stringstream ss;
	for (auto crate : crates)
	{
		ss << *(crate.end() - 1);
	}

	return ss.str();
}