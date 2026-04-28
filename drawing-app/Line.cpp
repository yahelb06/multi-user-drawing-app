#include "Line.h"

Line::Line(const std::pair<Coordinates, Coordinates>& line)
{
    this->_line = line;
}

std::pair<Coordinates, Coordinates> Line::getLine() const
{
    return this->_line;
}
