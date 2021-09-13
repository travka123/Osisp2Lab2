#include "Table.h"

#include <fstream>
#include <iostream>
#include <sstream>


std::vector<std::vector<std::string>> Table::_values;
int Table::_rowCount;
int Table::_columnsCount;

void Table::SetDataFromCSV(const char* filePath) {
	std::ifstream fin;
	fin.open(filePath);

	std::string line;
	while (std::getline(fin, line)) {
		std::vector<std::string> lineVec;
		std::stringstream lineStr(line);
		std::string cell;
		while (std::getline(lineStr, cell, ';')) {
			lineVec.push_back(cell);
		}
		_values.push_back(lineVec);
	}

	_rowCount = _values.size();
	if (_rowCount) {
		_columnsCount = _values[0].size();
	}

	fin.close();
}

void Table::Draw(HDC hDC, RECT clientRect) {
	const int horizontalPadding = 4;
	const int separatorWidth = 1;
	const int lineSpacing = 2;

	const float cellWidth = (float)(clientRect.right - separatorWidth) / _columnsCount;
	const int cellAvalibleWidth = (int)cellWidth - horizontalPadding * 2 - separatorWidth;

	TEXTMETRIC tm;
	GetTextMetrics(hDC, &tm);

	MoveToEx(hDC, 0, separatorWidth / 2, nullptr);
	LineTo(hDC, clientRect.right, separatorWidth / 2);

	int tableHeight = separatorWidth;
	for (int row = 0; row < _rowCount; row++) {
		int nextTableHeight = tableHeight;
		int columnX = separatorWidth + horizontalPadding;
		for (int column = 0; column < _columnsCount; column++) {
			int columY = tableHeight + lineSpacing;
			int fittedCharactersCount = 0;
			LPCSTR pStr = _values[row][column].c_str();
			while (fittedCharactersCount != _values[row][column].size()) {
				int fittingCharactersCount;
				SIZE size;
				GetTextExtentExPointA(hDC, pStr, _values[row][column].size() - fittedCharactersCount,
					cellAvalibleWidth, &fittingCharactersCount, nullptr, &size);

				if (fittingCharactersCount == 0) {
					break;
				}

				TextOutA(hDC, columnX, columY, pStr, fittingCharactersCount);

				pStr += fittingCharactersCount;
				fittedCharactersCount += fittingCharactersCount;
				columY += tm.tmHeight + lineSpacing;
			}
			columnX += cellWidth;

			int curNextTableHeight = columY + lineSpacing + separatorWidth;
			if (curNextTableHeight > nextTableHeight) {
				nextTableHeight = curNextTableHeight;
			}

		}

		tableHeight = nextTableHeight;

		int separatorY = tableHeight - separatorWidth / 2;
		MoveToEx(hDC, 0, separatorY, nullptr);
		LineTo(hDC, clientRect.right, separatorY);
	}

	float increment = cellWidth - (float)separatorWidth / 2;
	float separatorX = (float)separatorWidth / 2;
	while (separatorX < clientRect.right - increment) {
		MoveToEx(hDC, (int)separatorX, 0, nullptr);
		LineTo(hDC, (int)separatorX, tableHeight);
		separatorX += increment;
	}
	MoveToEx(hDC, clientRect.right - separatorWidth / 2 - 1, 0, nullptr);
	LineTo(hDC, clientRect.right - separatorWidth / 2 - 1, tableHeight);
	
}
