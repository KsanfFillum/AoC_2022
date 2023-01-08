#pragma once

#include <vector>
#include <string>
#include <sstream>

inline int sign(int val)
{
	return (0 < val) - (val < 0);
}

std::vector<std::string> tokenize(std::string str, const char* delimeter)
{
	std::vector<std::string> tokenized;
	size_t beginIndex = 0;
	size_t endIndex = 0;
	const size_t strLen = std::strlen(delimeter);

	while (endIndex < str.size())
	{
		endIndex = str.find(delimeter, beginIndex);
		if (endIndex == std::string::npos)
		{
			endIndex = str.size();
		}

		if (beginIndex != endIndex)
		{
			tokenized.push_back(str.substr(beginIndex, endIndex - beginIndex));
		}
		
		beginIndex = endIndex + strLen;
	}

	return tokenized;
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

bool operator ==(const Vector2& lhv, const Vector2& rhv)
{
	return lhv.X == rhv.X && lhv.Y == rhv.Y;
}

bool operator !=(const Vector2& lhv, const Vector2& rhv)
{
	return lhv.X != rhv.X || lhv.Y != rhv.Y;
}

namespace std
{
	string to_string(const Vector2& v)
	{
		char buffer[21];
		snprintf(buffer, 21, "[%i; %i]", v.X, v.Y);
		return buffer;
	}
}

// allow only numbers [0; 65535] for 4byte-int
struct VectorComparator
{
	inline bool operator()(const Vector2& lhv, const Vector2 rhv) const
	{
		constexpr size_t intSize = sizeof(int);
		return ((lhv.X << intSize * 4) + lhv.Y) < ((rhv.X << intSize * 4) + rhv.Y);
	}
};

// -------------Vector3-----------------

struct Vector3
{
	Vector3()
		: Vector3(0, 0, 0)
	{
	}

	Vector3(int x, int y, int z)
		: X(x)
		, Y(y)
		, Z(z)
	{
	}

	int X;
	int Y;
	int Z;
};

// -------------Heap-----------------

template<typename T, typename TPred = std::less<>>
class Heap
{
public:
	template<typename TIter>
	void insert(TIter begin, TIter end) 
	{
		Container.insert(Container.end(), begin, end);
		std::make_heap(Container.begin(), Container.end(), TPred());
	}
	
	void push(const T& value)
	{ 
		Container.push_back(value); 
		std::push_heap(Container.begin(), Container.end(), TPred());
	}
	
	T pop() 
	{ 
		T value = Container.front(); 
		std::pop_heap(Container.begin(), Container.end(), TPred());
		Container.pop_back();
		return value; 
	}

	T peek() const
	{
		return Container.front();
	}

	size_t size() const { return Container.size(); }


private:
	std::vector<T> Container;
	
};