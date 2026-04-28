#include "Paint.h"

Paint::Paint(const std::map<Line, std::string>& LinesInPaint, const std::string& name)
{
	this->_LinesInPaint = LinesInPaint;
}

std::map<Line, std::string> Paint::getPaint() const
{
	return std::map<Line, std::string>();
}

std::string Paint::getPaintName() const
{
	return this->_paintName;
}

void Paint::setPaintName(const std::string& name)
{
	this->_paintName = name;
}
