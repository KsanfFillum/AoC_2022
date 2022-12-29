#pragma once

#include "Common.h"

#undef CURRENT_DAY
#define CURRENT_DAY 12

namespace Day12
{
	using byte = char;
	constexpr byte North = 0b0001;
	constexpr byte West = 0b0010;
	constexpr byte South = 0b0100;
	constexpr byte East = 0b1000;

	struct Grid
	{
		size_t GetIndex(Vector2 pos) const { return pos.X + pos.Y * Width; }
		byte At(Vector2 pos) const { return Map[GetIndex(pos)]; }

		std::vector<byte> Map;
		int Width;
		int Height;
	};

	bool CanDescent(char current, char other) 
	{ 
		return (current - 1) <= other; 
	}
	Grid ConstructGrid(std::istream& stream, std::function<void(char, Vector2)> callback)
	{
		Grid grid;
		grid.Height = 0;
		std::vector<char> buffer;
		std::string line;

		while (!stream.eof())
		{
			std::getline(stream, line);
			buffer.insert(buffer.end(), line.begin(), line.end());
			grid.Height++;
		}

		for (int i = 0; i < buffer.size(); i++)
		{
			if (buffer[i] == 'E') { buffer[i] = 'z' + 1; }
			if (buffer[i] == 'S') { buffer[i] = 'a' - 1; }
		}

		grid.Width = static_cast<int>(line.size());
		grid.Map.resize(buffer.size(), 0);

		for (int y = 0; y < grid.Height; y++)
		{
			for (int x = 0; x < grid.Width; x++)
			{
				Vector2 position(x, y);
				char ch = buffer[grid.GetIndex(position)];
				callback(ch, position);

				byte mask = 0;
				
				if (x > 0 && CanDescent(ch, buffer[grid.GetIndex({ x - 1, y })]))
				{
					mask |= West;
				}

				if ((x < grid.Width - 1) && CanDescent(ch, buffer[grid.GetIndex({ x + 1, y })]))
				{
					mask |= East;
				}

				if (y > 0 && CanDescent(ch, buffer[grid.GetIndex({ x, y - 1 })]))
				{
					mask |= North;
				}

				if ((y < grid.Height - 1) && CanDescent(ch, buffer[grid.GetIndex({ x, y + 1})]))
				{
					mask |= South;
				}

				grid.Map[grid.GetIndex({ x, y })] = mask;
			}
		}

		return grid;
	}

	struct Node
	{
		int Score;
		Vector2 NodePos;
	};

	struct NodeComparator
	{
		bool operator()(const Node& lhv, const Node& rhv)
		{
			return lhv.Score > rhv.Score;
		}
	};

	inline bool ProcessPosition(const Vector2 offset, const Node oldNode, Vector2& endPos, std::map<Vector2, Vector2, VectorComparator>& chain, Heap<Node, NodeComparator>& heap, const std::function<bool(Vector2)>& predicate)
	{
		assert(offset != Vector2(0, 0));
		Vector2 newPos = oldNode.NodePos + offset;		
		if (chain.count(newPos))
		{
			return false;
		}

		Node n;
		n.NodePos = newPos;
		n.Score = oldNode.Score + 1;
		
		chain[newPos] = oldNode.NodePos;
		heap.push(n);

		if (predicate(newPos))
		{
			endPos = newPos;
			return true;
		}

		return false;
	}


	size_t Find(const Grid& grid, Vector2 startPoint, std::function<bool(Vector2)> predicate)
	{
		Heap<Node, NodeComparator> heap;
		std::map<Vector2, Vector2, VectorComparator> chain;

		heap.push({ 0, startPoint });
		chain[startPoint] = Vector2(-1, -1);

		Vector2 endPoint(-1, -1);
		while (heap.size() > 0)
		{
			Node currentNode = heap.pop();
			const Vector2 currentPos = currentNode.NodePos;

			byte mask = grid.At(currentPos);
			if ((mask & North) && ProcessPosition({ 0, -1 }, currentNode, endPoint, chain, heap, predicate)) { break; }
			if ((mask & South) && ProcessPosition({ 0,  1 }, currentNode, endPoint, chain, heap, predicate)) { break; }
			if ((mask & West ) && ProcessPosition({-1,  0 }, currentNode, endPoint, chain, heap, predicate)) { break; }
			if ((mask & East ) && ProcessPosition({ 1,  0 }, currentNode, endPoint, chain, heap, predicate)) { break; }
		}

		// did not find a way
		assert(chain.count(endPoint) > 0);

		size_t counter = 0;
		Vector2 currentPathPoint = endPoint;
		while (currentPathPoint != startPoint)
		{
			counter++;
			currentPathPoint = chain[currentPathPoint];
		}

		return counter;
	}
}

std::string PuzzleApproach<12, 1>::RunTest(std::istream& stream)
{
	Vector2 startPosition;
	Vector2 endPosition;

	Day12::Grid grid = Day12::ConstructGrid(stream, [&](char ch, Vector2 position)
		{
			if (ch == 'a' - 1) { startPosition = position; }
			if (ch == 'z' + 1) { endPosition = position;}
		});

	return std::to_string(Day12::Find(grid, endPosition, [startPosition](Vector2 testPosition) { return testPosition == startPosition; }));
}

std::string PuzzleApproach<12, 2>::RunTest(std::istream& stream)
{
	std::set<Vector2, VectorComparator> startPositions;
	Vector2 endPosition;

	Day12::Grid grid = Day12::ConstructGrid(stream, [&](char ch, Vector2 position)
		{
			if (ch <= 'a') { startPositions.insert(position); }
			if (ch == 'z' + 1) { endPosition = position; }
		});

	return std::to_string(Day12::Find(grid, endPosition, [startPositions](Vector2 testPosition) { return startPositions.count(testPosition) > 0; }));
}