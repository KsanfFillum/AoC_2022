#pragma once

#include <vector>
#include <string>
#include <sstream>

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

template<typename T>
struct ScopeSetter
{
	ScopeSetter(T& place, T value)
		: Place(place), Value(value)
	{
	}

	~ScopeSetter()
	{
		Place = Value;
	}

private:
	T& Place;
	T Value;
};

template<int bufferSize>
void CalculateSimpleMultipliers(unsigned int value, std::array<int, bufferSize> buffer, int& count)
{
	count = 0;
	if (value == 1)
	{
		return;
	}

	static int possibleMultipliers[] { 2, 3, 5, 7, 13, 17, 23 };
	for (auto multiplier : possibleMultipliers)
	{
		while (value % multiplier == 0)
		{
			buffer[count++] = multiplier;
			if ((value /= multiplier) == 1)
			{
				return;
			}
		}
	}

	// add more possible multipliers if assert was hitted
	assert(true);
}

// ----- VECTOR2 --------

struct Vector2
{
	Vector2() : Vector2(0, 0)
	{
	}

	Vector2(int x, int y)
		: X(x), Y(y)
	{
	}

	int X;
	int Y;
};

Vector2 operator+(const Vector2& lhv, const Vector2& rhv)
{
	return Vector2(lhv.X + rhv.X, lhv.Y + rhv.Y);
}

Vector2 operator-(const Vector2& lhv, const Vector2& rhv)
{
	return Vector2(lhv.X - rhv.X, lhv.Y - rhv.Y);
}

Vector2 operator*(const Vector2& lhv, int rhv)
{
	return Vector2(lhv.X * rhv, lhv.Y * rhv);
}