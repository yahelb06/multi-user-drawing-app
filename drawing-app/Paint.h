#pragma once
#include "Pch.h"
#include "Line.h"

class Paint
{
private:

	std::string _paintName;
	std::vector<Line> _LinesInPaint;

public:
	Paint() = default;
	Paint(const std::string& paintName);
	Paint(const std::vector<Line>& LinesInPaint, const std::string& name);

	std::vector<Line> getPaint() const;
	void setPaintLines(const std::vector<Line>& LinesInPaint);

	std::string getPaintName() const;
	void setPaintName(const std::string& name);

	void cleanPaint();
};