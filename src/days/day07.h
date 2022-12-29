#pragma once

#include "Common.h"
#include <map>
#include <functional>
#include <cassert>

#undef CURRENT_DAY
#define CURRENT_DAY 7

namespace Day07
{
	struct File
	{
		std::string Name;
		size_t Size;
	};

	struct Directory
	{
		Directory(std::string name, Directory* parent = nullptr)
			: Name(name)
			, Parent(parent)
			, CachedSize(0)
		{
		}

		Directory* Parent;

		std::string Name;

		std::map<std::string, File> Files;
		std::map<std::string, Directory> SubDirectories;

		Directory* GetSubDirectory(std::string name) { auto it = SubDirectories.find(name); return it == SubDirectories.end() ? nullptr : &it->second; }
		size_t GetSize() const { return CachedSize; }
		void CalculateSize();

	private:

		size_t CachedSize;
	};

	struct Processor
	{
		Processor()
			: Root("/")
			, Current(&Root)
		{
		}

		void Next(std::istream& stream);
		void ProcessCD(std::vector<std::string> lines);
		void ProcessLS(std::vector<std::string> lines);

		Directory* GetRoot() { return &Root; }

	private: 

		Directory Root;
		Directory* Current;

	};

	void Directory::CalculateSize()
	{
		CachedSize = 0;

		for (auto& filePair : Files)
		{
			CachedSize += filePair.second.Size;
		}

		for (auto& dirPair : SubDirectories)
		{
			dirPair.second.CalculateSize();
			CachedSize += dirPair.second.GetSize();
		}
	}

	std::vector<std::string> tokenize(std::string str, char delimeter)
	{
		std::stringstream sstream(str);
		std::string buffer;
		std::vector<std::string> tokenized;

		while (!sstream.eof())
		{
			std::getline(sstream, buffer, delimeter);
			if (!buffer.empty())
			{
				tokenized.push_back(buffer);
			}
		}

		return tokenized;
	}

	void Processor::ProcessCD(std::vector<std::string> lines)
	{
		std::string str = lines[0].substr(5);
		std::vector<std::string> tokens = tokenize(str, '/');
		if (*str.begin() == '/')
		{
			Current = &Root;
		}

		for (auto token : tokens)
		{
			if (token == "..")
			{
				Current = Current->Parent;
			}
			else 
			{
				auto foundDir = Current->GetSubDirectory(token);
				assert(foundDir != nullptr);
				Current = foundDir;
			}
		}
	}

	void Processor::ProcessLS(std::vector<std::string> lines)
	{
		for (int i = 1; i < lines.size(); i++)
		{
			const size_t delimeterIndex = lines[i].find(' ');
			if (lines[i].substr(0, 3) == "dir")
			{
				std::string name = lines[i].substr(4);
				Current->SubDirectories.emplace(name, Directory(name, Current));
			}
			else
			{
				std::string name = lines[i].substr(delimeterIndex + 1);
				size_t size = stoi(lines[i].substr(0, delimeterIndex));
				Current->Files.emplace(name, File{ name, size });
			}
		}
	}

	void Processor::Next(std::istream& stream)
	{
		static std::map<std::string, std::function<void(Processor*, std::vector<std::string>)>> ProcessorFunctions(
			{
				{ "$ cd", &Processor::ProcessCD },
				{ "$ ls", &Processor::ProcessLS },
			});

		static std::vector<std::string> lines;
		static std::string line;

		lines.clear();
		do 
		{
			std::getline(stream, line);
			lines.emplace_back(line);
		} while (!stream.eof() && stream.peek() != '$');

		ProcessorFunctions[lines[0].substr(0, 4)](this, lines);
	}

	size_t CalculateRecursivePuzzle1(Directory* dir, int threshold)
	{
		size_t size = dir->GetSize() <= threshold ? dir->GetSize() : 0;
		for (auto& subDir : dir->SubDirectories)
		{
			size += CalculateRecursivePuzzle1(&subDir.second, threshold);
		}

		return size;
	}

	void CalculateRecursivePuzzle2Internal(Directory* dir, size_t reqSizeToFreedUp, size_t& best)
	{
		size_t size = dir->GetSize();
		size_t score = size - reqSizeToFreedUp;
		if (best > score && score > 0)
		{
			best = score;
		}

		for (auto& subDir : dir->SubDirectories)
		{
			CalculateRecursivePuzzle2Internal(&subDir.second, reqSizeToFreedUp, best);
		}
	}

	size_t CalculateRecursivePuzzle2(Directory* Dir, size_t totalSize, size_t reqSize)
	{
		size_t reqSizeToFreedUp = reqSize - (totalSize - Dir->GetSize());
		size_t best = std::numeric_limits<size_t>::max();
		CalculateRecursivePuzzle2Internal(Dir, reqSizeToFreedUp, best);
	
		return reqSizeToFreedUp + best;
	}
}

template<>
std::string PuzzleApproach<7, 1>::RunTest(std::istream& stream)
{
	using namespace Day07;

	Processor processor;
	while (!stream.eof())
	{
		processor.Next(stream);
	}

	processor.GetRoot()->CalculateSize();
	size_t sum = CalculateRecursivePuzzle1(processor.GetRoot(), 100000);
	return std::to_string(sum);
}

template<>
std::string PuzzleApproach<7, 2>::RunTest(std::istream& stream)
{
	using namespace Day07;

	Processor processor;
	while (!stream.eof())
	{
		processor.Next(stream);
	}
	processor.GetRoot()->CalculateSize();

	constexpr size_t totalSize = 70000000;
	constexpr size_t reqSize = 30000000;

	size_t sum = CalculateRecursivePuzzle2(processor.GetRoot(), totalSize, reqSize);
	return std::to_string(sum);
}