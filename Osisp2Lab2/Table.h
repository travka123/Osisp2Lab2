#pragma once

#include <Windows.h>
#include <vector>
#include <string>

class Table {
public:
	static void SetDataFromCSV(const char * filePath);
	static void Draw(HDC hDC, RECT clientRect);

private:
	static std::vector<std::vector<std::string>> _values;
	static int _rowCount;
	static int _columnsCount;
};

