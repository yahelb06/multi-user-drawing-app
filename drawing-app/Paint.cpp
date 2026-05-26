#include "Paint.h"

Paint::Paint(const std::string& paintName) : Paint({}, paintName)
{
}

Paint::Paint(const std::vector<Line>& LinesInPaint, const std::string& name) : _paintName(name), _LinesInPaint(LinesInPaint)
{
}

std::vector<Line> Paint::getPaintLines() const
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

void Paint::AddLine(const std::vector<Line>& linesToAdd)
{
	this->_LinesInPaint.insert(this->_LinesInPaint.end(), linesToAdd.begin(), linesToAdd.end());
}

void Paint::cleanPaint()
{
	this->_LinesInPaint.clear();
}

