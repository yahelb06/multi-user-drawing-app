#include "Paint.h"

Paint::Paint(const std::vector<Line>& LinesInPaint)
{
	this->_LinesInPaint = LinesInPaint;
}

std::vector<Line> Paint::getPaint() const
{
	return this->_LinesInPaint;
}

std::string Paint::getPaintName() const
{
	return this->_paintName;
}

void Paint::setPaintName(const std::string& name)
{
	this->_paintName = name;
}
