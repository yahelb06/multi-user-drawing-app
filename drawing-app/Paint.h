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
	Paint(const std::vector<Line>& LinesInPaint);

	std::vector<Line> getPaint() const;

	std::string getPaintName() const;
	void setPaintName(const std::string& name);
};