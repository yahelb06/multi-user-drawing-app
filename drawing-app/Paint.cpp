#include "Paint.h"

Paint::Paint(const std::string& paintName) : _paintName(paintName)
{
}

Paint::Paint(const std::vector<Line>& LinesInPaint, const std::string& name) : _paintName(name), _LinesInPaint(LinesInPaint)
{
}

std::vector<Line> Paint::getPaint() const
{
	return this->_LinesInPaint;
}

void Paint::setPaintLines(const std::vector<Line>& LinesInPaint)
{
	this->_LinesInPaint = LinesInPaint;
}

std::string Paint::getPaintName() const
{
	return this->_paintName;
}

void Paint::setPaintName(const std::string& name)
{
	this->_paintName = name;
}

void Paint::cleanPaint()
{
	this->_LinesInPaint.clear();
}

