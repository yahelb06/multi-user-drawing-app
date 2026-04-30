#pragma once
#include "Pch.h"

struct Coordinates
{
	std::pair<unsigned int, unsigned int> coordinates;

	Coordinates(unsigned int x, unsigned int y);
	
	Coordinates() = default;
};

class Line
{
private:

	std::pair<Coordinates, Coordinates> _line;
	std::string _color;

public:
	Line(const std::pair<Coordinates, Coordinates>& line, const std::string& color);

	std::pair<Coordinates, Coordinates> getLine() const;
};