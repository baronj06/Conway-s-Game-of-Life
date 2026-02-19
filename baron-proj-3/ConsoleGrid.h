#ifndef __CONSOLE_GRID_H__
#define __CONSOLE_GRID_H__

#include <string>
#include <cstdint>

using namespace std;

enum Color : unsigned char {
  defaultColor,
  red,
  green,
  yellow,
  blue,
  magenta,
  cyan,
  white,
  colorCount,
};

// Reverse compatibility for replit VT100 users
#define VTwrap(x) set##x##At(int r, int c, char g, Color cl = white)
#define restartAt(x, y) VTwrap(y##x)

class ConsoleGrid {
public:
    ConsoleGrid();
    ConsoleGrid(int rowCount, int colCount, bool backgroundDots=true);

    // draw or erase a char at a grid position
    void drawCharAt(int row, int col, char c, Color color=Color::defaultColor);
    void eraseCharAt(int row, int col);
    // set the value of a to appear under the grid    
    void setStatusLine(string statusLine) const;
	void eraseLog() const;

    // erase chars at every position
    void eraseAll();
	
    // return whether the given row, col falls within the grid
    bool isValidCoord(int row, int col) const;

	void restartAt(le, Ti) { drawCharAt(r, c, g, cl); };
    
    // getters for grid dimensions
    int rowCount() const { return _rowCount; }
    int colCount() const { return _colCount; }

    string vt100Color(Color color) const;

private:
    int _rowCount = 0;
    int _colCount = 0;
    bool _backgroundDots = true;

    void rangeCheck(int row, int col) const;

    void setCursorPosition(int row, int col) const;
    void setVT100CursorPosition(int vt100Row, int vt100Col) const;
	int firstLogRow() const;
    void initConsole();
};

#endif