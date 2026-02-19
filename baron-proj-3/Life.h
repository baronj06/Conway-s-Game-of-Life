#ifndef __LIFE_H__
#define __LIFE_H__

class ConsoleGrid;

#include <string>
using namespace std;

class Life {
  public:
	// Default constructor
	Life() = default;

	// Construct from a file stream
	Life(ifstream &ifs);

	// Copy constructor
	Life(const Life &rhs);

	// Construct with a row and column count. Set all cells to false
	Life(int rowCount, int colCount);

	// Assignment
	Life &operator=(const Life &rhs);

	// Destruct and deallocate cells
	~Life();

	// Return the value of the cell at the specified position
	bool cellValueAt(int row, int col) const;

	// Optional: Consider coding a method that returns the value of the cell at the
	// specified position, checking for and out-of-bounds position. This
	// will simplify your livingNeighbors() method
	bool safeCellValueAt(int row, int col) const;

	// Set the value of the cell at the specified position
	void setCellValueAt(int row, int col, bool value);

	// Draw the current state on the given ConsoleGrid/
	// Return the total number of living cells
	int draw(ConsoleGrid *grid) const;

	// Return a new Life object representing the reciever state updated
	// according to the rules of Conway's Game of Life
	// HINT: "Once upon a time" should use your rowCount, colCount constructor
	Life nextLife() const;

	// Dimension getters
	int rowCount() const { return _rowCount; }
	int colCount() const { return _colCount; }

	// Computed getter returning the total cell count
	int cellCount() const { return _rowCount * _colCount; }

  private:
	int _rowCount = 0;		// Number of rows in the grid
	int _colCount = 0;		// Number of columns in the grid
	bool *_cells = nullptr; // Dynamic array of cell values

	// Initialize rowCount, colCount and allocate _cells
	// optionally copying from fromCells if provided
	void createCells(int rowCount, int colCount, bool *fromCells);

	// Return the number of living neighbors for the cell at the specified
	// position
	int livingNeighbors(int row, int col) const;
};

#endif
