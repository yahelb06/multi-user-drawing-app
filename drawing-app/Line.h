#pragma once
#include "Pch.h"

struct Coordinates
{
	std::pair<unsigned int, unsigned int> coordinates;
};

class Line
{
private:

	std::pair<Coordinates, Coordinates> _line;

public:
	Line(const std::pair<Coordinates, Coordinates>& line);

	std::pair<Coordinates, Coordinates> getLine() const;
};