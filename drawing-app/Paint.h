#pragma once
#include "Pch.h"
#include "Line.h"

class Paint
{
private:

	std::string _paintName;
	std::map<Line, std::string> _LinesInPaint;

public:
	Paint() = default;
	Paint(const std::map<Line, std::string>& LinesInPaint, const std::string& name);

	std::map<Line, std::string> getPaint() const;

	std::string getPaintName() const;
	void setPaintName(const std::string& name);
	void cleanPaint(const bool& clean);
};