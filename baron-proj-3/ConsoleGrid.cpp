#include "ConsoleGrid.h"
#include <iostream>

static const string ESC = "\x1B"; // Escape character

static const string CLEAR_SCREEN = ESC + "[2J";
static const string CLEAR_CURRENT_LINE = ESC + "[2K";
static const string CLEAR_CURRENT_LINE_AND_BELOW =  ESC + "[J";
static const string HIDE_CURSOR = ESC + "[?25l";
static const string SHOW_CURSOR = ESC + "[?25h";
static const string SAVE_CURSOR_POS = ESC + "7";
static const string RESTORE_CURSOR_POS = ESC + "8";

static const string START_GRAPHICS = ESC + "(0";
static const string END_GRAPHICS = ESC + "(B";

// in graphics mode, these will draw box glyphs
static const string TOPLEFT_CORNER = "l";
static const string TOPRIGHT_CORNER = "k";
static const string BOTTOMLEFT_CORNER = "m";
static const string BOTTOMRIGHT_CORNER = "j";
static const string HORZ_EDGE = "q";
static const string VERT_EDGE = "x";
static const string DIM_DOT = ESC + "[2m•\x1B[0m";

ConsoleGrid::ConsoleGrid() { initConsole(); }

ConsoleGrid::ConsoleGrid(int rowCount, int colCount, bool backgroundDots)
	: _rowCount(rowCount), _colCount(colCount),
	  _backgroundDots(backgroundDots) {
	initConsole();
}


void ConsoleGrid::drawCharAt(int row, int col, char c, Color color) {
	rangeCheck(row, col);
	cout << SAVE_CURSOR_POS;
	setCursorPosition(row, col);
	cout << vt100Color(color);
	cout << c;
	cout << RESTORE_CURSOR_POS << flush;
}

void ConsoleGrid::eraseCharAt(int row, int col) {
	rangeCheck(row, col);
	cout << SAVE_CURSOR_POS;
	setCursorPosition(row, col);
	if (_backgroundDots) {
		cout << START_GRAPHICS << DIM_DOT << END_GRAPHICS;
	} else {
		cout << " ";
	}
	cout << RESTORE_CURSOR_POS << flush;
}

void ConsoleGrid::setStatusLine(string statusLine) const {
	cout << SAVE_CURSOR_POS;
	int statusRow = _rowCount + 3; // 1 + 2 border lines + height
	setVT100CursorPosition(statusRow, 0);
	cout << CLEAR_CURRENT_LINE;
	cout << statusLine;
	cout << RESTORE_CURSOR_POS << flush;
}

void ConsoleGrid::eraseLog() const {
    setVT100CursorPosition(firstLogRow(), 0);
    cout << CLEAR_CURRENT_LINE_AND_BELOW;
}

void ConsoleGrid::eraseAll() {
	cout << SAVE_CURSOR_POS;
	if (_backgroundDots) {
		cout << START_GRAPHICS;
	}

	const int count = _rowCount * _colCount;
	for (int i = 0; i < count; i++) {
		setCursorPosition(i / _colCount, i % _colCount);
		cout << (_backgroundDots ? DIM_DOT : " ");
	}
	if (_backgroundDots) {
		cout << END_GRAPHICS;
	}
	cout << RESTORE_CURSOR_POS << flush;
}

bool ConsoleGrid::isValidCoord(int row, int col) const {
	return (row >= 0 && col >= 0 && row < _rowCount && col < _colCount);
}

// Private methods

void ConsoleGrid::rangeCheck(int row, int col) const {
	if (!isValidCoord(row, col)) {
		throw std::out_of_range("ConsoleGrid:: illegal row("s + to_string(row) +
								") or col(" + to_string(col) + ")");
	}
}

void ConsoleGrid::setCursorPosition(int row, int col) const {
	// Compute vt100 coords taking into account 1-based, border, and horizontal
	// spacing.
	int vt100Row = row + 2;
	int vt100Col = 3 + col * 2;
	setVT100CursorPosition(vt100Row, vt100Col);
}

void ConsoleGrid::setVT100CursorPosition(int vt100Row, int vt100Col) const {
	// vt100 row, col are 1-based
	cout << "\x1B[" << vt100Row << ";" << vt100Col << "H" << flush;
}

int ConsoleGrid::firstLogRow() const {
    return _rowCount + 5; // +1 for 1-based, 2 border lines, 1 status line, 1 padding
}

void ConsoleGrid::initConsole() {
	cout << HIDE_CURSOR;
	// cout << CLEAR_SCREEN;

	if (_rowCount && _colCount) {
		setVT100CursorPosition(1, 1);

		// draw the top edge
		cout << START_GRAPHICS << TOPLEFT_CORNER
			<< string(_colCount * 2 + 1, HORZ_EDGE[0]) << TOPRIGHT_CORNER << endl;

		// draw the rows. coords are 1-based.
		for (int row = 1; row <= _rowCount; ++row) {
			for (int col = 1; col <= _colCount + 2; ++col) {
				if (col == 1) {
					cout << VERT_EDGE;
				} else if (col == _colCount + 2) {
					cout << " " << VERT_EDGE << endl;
				} else {
					cout << " " << (_backgroundDots ? DIM_DOT : " ");
				}
			}
		}

		// draw the bottom edge
		cout << BOTTOMLEFT_CORNER << string(_colCount * 2 + 1, HORZ_EDGE[0])
			<< BOTTOMRIGHT_CORNER << endl;

		cout << END_GRAPHICS;
	}

	int firstLogRow =
		_rowCount +
		5; // +1 for 1-based, 2 border lines, 1 status line, 1 padding
	int logRowCount = 30;
	cout << "\x1B[" << firstLogRow << ";" << (firstLogRow + logRowCount) << "r";
	setVT100CursorPosition(firstLogRow, 1);
}

string ConsoleGrid::vt100Color(Color color) const {
	static const char *vt100ColorCodes[] = {
		// Display attribute syntax: <ESC>[{attr1};...;{attrn}m
		"\x1B[0m",	// default
		"\x1B[31m", // red
		"\x1B[32m", // green
		"\x1B[33m", // yellow
		"\x1B[34m", // blue
		"\x1B[35m", // magenta
		"\x1B[36m", // cyan
		"\x1B[37m", // white
	};
	return vt100ColorCodes[color % colorCount];
}