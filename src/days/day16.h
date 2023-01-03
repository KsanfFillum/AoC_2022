#pragma once

#include "Common.h"
#include "Helpers.h"
#include <optional>

#undef CURRENT_DAY
#define CURRENT_DAY 16

//template<>
//constexpr char RunPuzzleTraits<16>::Value = RunPuzzleTraitsConstant::RunTest;

namespace Day16
{
	struct Node
	{
		int FlowRate;
		std::vector<int> DestinationIndicies;
		int Comparant;
	};

	inline int toComparant(char first, char second)
	{
		return first + (second << 8);
	}

	template<typename TVal, typename TComp, typename Pred>
	std::optional<size_t> binarySearchIndex(const std::vector<TVal>& vector, const TComp& data, Pred pred)
	{
		size_t lowerBound = 0;
		size_t upperBound = vector.size();

		while (upperBound != lowerBound)
		{
			size_t index = lowerBound + (upperBound - lowerBound) / 2;
			const TComp comparant = pred(vector[index]);
			if (comparant == data)
			{
				return index;
			}
			else if (comparant < data)
			{
				lowerBound = index + 1;
			}
			else
			{
				upperBound = index;
			}
		}

		return std::optional<size_t>();
	}

	std::vector<Node> parseInput(std::istream& stream)
	{
		std::vector<Node> nodes;
		std::vector<std::string> lines;
		std::string strBuffer;
		while (!stream.eof())
		{
			std::getline(stream, strBuffer);
			lines.push_back(strBuffer);
		}

		nodes.reserve(lines.size());
		char charBuffer[8];
		for (auto& line : lines)
		{
			Node n;
			n.Comparant = toComparant(line[6], line[7]);

			const int firstIndex = 23;
			size_t secondIndex = line.find(';', firstIndex);
			std::memcpy(charBuffer, line.c_str() + firstIndex, secondIndex - firstIndex);
			charBuffer[secondIndex - firstIndex] = '\0';
			n.FlowRate = atoi(charBuffer);

			nodes.emplace_back(n);
		}

		std::sort(nodes.begin(), nodes.end(), [](const Node& lhv, const Node& rhv) { return lhv.Comparant < rhv.Comparant; });

		for (auto& line : lines)
		{
			auto index = binarySearchIndex(nodes, toComparant(line[6], line[7]), [](const Node& n) { return n.Comparant; });
			assert(index.has_value());
			Node& currentNode = nodes[index.value()];

			size_t beginIndex = line.find("valve", 1);
			beginIndex = line.find(' ', beginIndex) + 1;
			while (line.size() > beginIndex)
			{
				index = binarySearchIndex(nodes, toComparant(line[beginIndex], line[beginIndex + 1]), [](const Node& n) { return n.Comparant; });
				assert(index.has_value());
				currentNode.DestinationIndicies.push_back((int)index.value());
				beginIndex += 4;
			}
		}

		return nodes;
	}

	struct Puzzle1State
	{
		int64_t possibleValues = 0;
		int reducedPressure = 0;
		int currentNode;
		int lastNode;
	};

	struct Puzzle2State
	{
		int64_t possibleValues = 0;
		int reducedPressure = 0;

		int elephantCurrentNode;
		int elephantLastNode;

		int currentNode;
		int lastNode;
	};

	void monteCarloStep(std::vector<Node>& nodes, std::vector<Puzzle1State>& states, int turnsLeft)
	{
		std::vector<Puzzle1State> newStates;
		for (const auto& state : states)
		{
			if (state.possibleValues & (1LL << state.currentNode))
			{
				Puzzle1State s = state;
				s.possibleValues &= ~(1LL << state.currentNode);
				s.reducedPressure += turnsLeft * nodes[state.currentNode].FlowRate;
				s.lastNode = -1; // allow to come back after turn a valve
				newStates.emplace_back(s);
			}

			for (int dest : nodes[state.currentNode].DestinationIndicies)
			{
				if (dest != state.lastNode)
				{
					Puzzle1State s = state;
					s.currentNode = dest;
					s.lastNode = state.currentNode;
					newStates.emplace_back(s);
				}
			}
		}

		std::swap(newStates, states);
	}

	void monteCarloStep(std::vector<Node>& nodes, std::vector<Puzzle2State>& states, int turnsLeft)
	{
		std::vector<Puzzle2State> newStates;
		for (const Puzzle2State& state : states)
		{
			bool possibleToTurnValve = state.possibleValues & (1LL << state.currentNode);
			bool elephantPossibleToTurnValve = (state.possibleValues & 1LL << state.elephantCurrentNode) && (state.elephantCurrentNode != state.currentNode);

			if (possibleToTurnValve)
			{
				Puzzle2State s = state;
				s.possibleValues &= ~(1LL << state.currentNode);
				s.reducedPressure += turnsLeft * nodes[state.currentNode].FlowRate;
				s.lastNode = -1; // allow to come back after turn a valve
				s.elephantLastNode = -1;

				if (elephantPossibleToTurnValve)
				{
					Puzzle2State s2 = s;
					s2.possibleValues &= ~(1LL << state.elephantCurrentNode);
					s2.reducedPressure += turnsLeft * nodes[state.elephantCurrentNode].FlowRate;	
					newStates.emplace_back(s2);
				}

				for (int dest : nodes[state.elephantCurrentNode].DestinationIndicies)
				{
					s.elephantLastNode = s.elephantCurrentNode;
					s.elephantCurrentNode = dest;
					newStates.emplace_back(s);
				}
			}

			if (elephantPossibleToTurnValve)
			{
				Puzzle2State s = state;
				s.possibleValues &= ~(1LL << state.elephantCurrentNode);
				s.reducedPressure += turnsLeft * nodes[state.elephantCurrentNode].FlowRate;
				s.elephantLastNode = -1; // allow to come back after turn a valve
				s.lastNode = -1;

				for (int dest : nodes[state.currentNode].DestinationIndicies)
				{
					s.lastNode = s.currentNode;
					s.currentNode = dest;
					newStates.emplace_back(s);
				}
			}

			for (int dest : nodes[state.currentNode].DestinationIndicies)
			{
				if (dest != state.lastNode)
				{
					Puzzle2State s = state;
					s.currentNode = dest;
					s.lastNode = state.currentNode;

					for (int elephDest : nodes[state.elephantCurrentNode].DestinationIndicies)
					{
						if (elephDest != state.elephantLastNode)
						{
							s.elephantCurrentNode = elephDest;
							s.elephantLastNode = state.elephantLastNode;
							newStates.emplace_back(s);
						}
					}
				}
			}
		}

		std::swap(newStates, states);
	}

	template<typename TState>
	void reduceStates(std::vector<TState>& states)
	{
		constexpr int targetStepsCount = 5000;
		if (states.size() > targetStepsCount)
		{
			std::sort(states.begin(), states.end(), [](const TState& lhv, const TState& rhv) { return lhv.reducedPressure > rhv.reducedPressure; });
			states.resize(targetStepsCount);
		}
	}

	int64_t calculateMask(const std::vector<Node>& nodes)
	{
		int64_t mask = 0;
		for (int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i].FlowRate > 0)
			{
				mask |= 1LL << i;
			}
		}

		return mask;
	}
}

std::string PuzzleApproach<16, 1>::RunTest(std::istream& stream)
{
	std::vector<Day16::Node> nodes = Day16::parseInput(stream);

	assert(nodes.size() < 64);

	std::vector<Day16::Puzzle1State> states;
	Day16::Puzzle1State initialState;
	initialState.currentNode = 0;
	initialState.lastNode = -1;
	initialState.possibleValues = calculateMask(nodes);

	states.push_back(initialState);

	int remainingSteps = 30;
	while (remainingSteps > 0)
	{
		Day16::monteCarloStep(nodes, states, --remainingSteps);
		Day16::reduceStates(states);
	}
	
	int score = 0;
	int reducedPressure = 0;

	for (auto& state : states)
	{
		score = std::max(state.reducedPressure, score);
	}

	return std::to_string(score);
}

std::string PuzzleApproach<16, 2>::RunTest(std::istream& stream)
{
	std::vector<Day16::Node> nodes = Day16::parseInput(stream);

	assert(nodes.size() < 64);
	
	std::vector<Day16::Puzzle2State> states;
	Day16::Puzzle2State initialState;
	initialState.currentNode = initialState.elephantCurrentNode = 0;
	initialState.lastNode = initialState.elephantLastNode = -1;
	initialState.possibleValues = calculateMask(nodes);

	states.push_back(initialState);

	int remainingSteps = 26;
	while (remainingSteps > 0)
	{
		Day16::monteCarloStep(nodes, states, --remainingSteps);
		Day16::reduceStates(states);	
	}

	int score = 0;
	int reducedPressure = 0;

	for (auto& state : states)
	{
		score = std::max(state.reducedPressure, score);
	}

	return std::to_string(score);
}