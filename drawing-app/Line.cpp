#include "Line.h"

Line::Line(const std::pair<Coordinates, Coordinates>& line, const std::string& color) : _line(line), _color(color)
{
}

std::pair<Coordinates, Coordinates> Line::getLine() const
{
    return this->_line;
}

Coordinates::Coordinates(unsigned int x, unsigned int y) : coordinates(std::make_pair(x, y ))
{
}
