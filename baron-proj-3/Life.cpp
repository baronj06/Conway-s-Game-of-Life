#include "ConsoleGrid.h"
#include "Life.h"

#include <fstream>
#include <iostream>
#include <sstream>

// Code the Life methods here

void Life::createCells(int rowCount, int colCount, bool *fromCells){
	_rowCount = rowCount;
	_colCount = colCount;
	_cells = new bool[_rowCount*_colCount]{false};

	if(fromCells != nullptr){
		for(int i = 0; i < rowCount * colCount; i++){
			_cells[i] = fromCells[i];
		}
	}
}

int Life::livingNeighbors(int row, int col) const{
	int count = 0;

	for(int i = row - 1; i <= row + 1; i++){
		for(int j = col -1; j <= col + 1; j++){
			if(i >= 0 && i < _rowCount && j >= 0 && j < _colCount){
				if(safeCellValueAt(i,j)){
				 	++ count;
				}
			}
		}
	}
	if(safeCellValueAt(row,col)){ // DJH: -0 The unsafe getter works here
		-- count;
	}

	return count;
}


Life::Life(ifstream &ifs){
	// DJH: Constructors usually attempt to assign a value for every member variable in the class.  Does this constructor do that?

	int row = 0,col = 0;
	string line;
	
	ifs >> row >> col;


	// DJH: -1 Do you see a way to avoid the previous two lines of code?
	// BC (FIXED): I delete two lines and chaged it to call createCells(row,col,nullptr). 
	createCells(row,col,nullptr);

	getline(ifs,line);

	// DJH: this initializes line from the input stream, but what does it do with line?
	// DJH: this reads only one line from the file.

	for(int i = 0; i < row; i ++){
		getline(ifs,line);
		for(int j = 0; j < col; j++){
			bool alive = false;
			
			if(line[j] == 'O'){
				alive = true;
			}
			
			setCellValueAt(i,j,alive);
			
		}
	}

}


Life::Life(const Life &rhs){
	createCells(rhs._rowCount,rhs._colCount,rhs._cells);
}

Life::Life(int rowCount, int colCount){
	createCells(rowCount,colCount,nullptr);
}

Life& Life::operator=(const Life &rhs){
	if(this == &rhs) return *this;

	/*
	_rowCount = rhs._rowCount;
	_colCount = rhs._colCount;
	_cells = new bool[_rowCount*_colCount];
	*/
	
	createCells(rhs._rowCount,rhs._colCount,rhs._cells);

	return *this;
}

Life::~Life(){
	delete[] _cells;
	_cells = nullptr;
}

bool Life::cellValueAt(int row, int col) const{
	return _cells[row * _colCount + col];
}

// DJH: -2 Early returns.
// BC (FIXED): I made other variable and return at end of code.
bool Life::safeCellValueAt(int row, int col) const{
	bool result = false;
	if(row >= 0 && row < _rowCount && col >= 0 && col < _colCount){
		result = cellValueAt(row,col);
	} 
	return result;
}

void Life::setCellValueAt(int row, int col, bool value){
	_cells[row*_colCount+col] = value;
}

int Life::draw(ConsoleGrid *grid) const{
	int alive = 0;
	for(int i = 0; i < _rowCount; i++){
		for(int j = 0; j < _colCount; j++){
			bool currentCell = cellValueAt(i,j);
			if(currentCell){
				++alive;

				grid-> drawCharAt(i,j,'O',green);
			}
			else{
				grid-> eraseCharAt(i,j);
			}
		}
	}

	return alive;
}

Life Life::nextLife() const{
	Life next(_rowCount,_colCount);
	// DJH: -2 We can avoid a few lines in this method by noticing
	// that the constructor properly initializes _cells to all false
	// BC (FIXED): Removed unnecessary false-assignments.

	for(int i = 0; i < _rowCount; i++){
		for(int j = 0; j < _colCount; j++){
			bool currentCell = cellValueAt(i,j);
			int neighbors = livingNeighbors(i,j); 

			if(!currentCell && neighbors == 3){
				next.setCellValueAt(i,j,true);
			}
			else if (currentCell && (neighbors == 2 || neighbors == 3)){
				next.setCellValueAt(i,j,true);
			}

		}
	}

	return next;
	
}

// DJH: 95/100. Well done, Baron! 
// Eligible for fix

// DJH: Fails to run on any input -60. I will deduct -5 instead
// DJH: No "fix" comments found
// Revised to 90/100

/*
| 지시문 표현                                                                    | 우리가 코드에서 해야 할 것                                                                              |
|-------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------|
| Life manages an array of bools called _cells                                  | private: int _rowCount=0, _colCount=0; bool* _cells=nullptr;                                           |
| 2D grid but flat array                                                        | 인덱스: row * _colCount + col 사용                                                                     |
| createCells(...) initializes rows, cols, allocates cells                      | _rowCount=rowCount; _colCount=colCount; _cells = new bool[rowCount*colCount]{false};                   |
| optionally copy from fromCells                                                | if(fromCells != nullptr) for i 0..row*col-1: _cells[i] = fromCells[i];                                 |
|                                                                                                                                                |
| Construct from file stream: Life(ifstream &ifs)                               | 파일에서 row,col 읽고 → createCells(row,col,nullptr); 줄 단위로 읽어서 'O'면 true 설정                  |
| first line of file has row, col                                               | ifs >> row >> col; 그 다음 getline 한 번 해서 줄바꿈 소비                                             |
| each following line is a row of 'O' or '.' / ' '                              | getline(ifs, line); line[j] == 'O' → alive=true → setCellValueAt(i,j,alive);                           |
|                                                                                                                                                |
| Copy constructor: creates deep copy                                           | Life::Life(const Life& rhs){ createCells(rhs._rowCount,rhs._colCount,rhs._cells); }                    |
| Construct with rowCount, colCount, all false                                  | Life::Life(int r,int c){ createCells(r,c,nullptr); }                                                   |
| Assignment operator, avoid self assignment                                    | if(this==&rhs) return *this; (기존 _cells 해제 후) createCells(rhs._rowCount,rhs._colCount,rhs._cells);|
| Destructor deallocates cells                                                  | delete[] _cells; _cells=nullptr;                                                                       |
|                                                                                                                                                |
| cellValueAt(row,col) returns value (no bounds check)                          | return _cells[row * _colCount + col];                                                                  |
| safeCellValueAt(row,col) with bounds check                                    | if(row,col 범위 안이면) result=cellValueAt(row,col); else false 반환                                   |
| setCellValueAt(row,col,value)                                                 | _cells[row * _colCount + col] = value;                                                                 |
|                                                                                                                                                |
| livingNeighbors(row,col): count 8 neighbors only                              | i=row-1..row+1, j=col-1..col+1 반복, 범위 체크 후 safeCellValueAt(i,j) 참이면 ++count;                |
|                                                                               | 마지막에 if(safeCellValueAt(row,col)) --count; (자기 자신 빼기)                                       |
|                                                                                                                                                |
| draw(ConsoleGrid* grid): draw live cells and count                            | 이중 for, currentCell = cellValueAt(i,j);                                                              |
| live면 grid->drawCharAt(i,j,'O',green); dead면 grid->eraseCharAt(i,j);        | 동시에 alive 개수 세서 마지막에 반환                                                                   |
|                                                                                                                                                |
| nextLife(): 새 세대 Life 리턴                                                 | Life next(_rowCount,_colCount); (생성자가 이미 전부 false로 초기화)                                    |
| apply Conway rules using livingNeighbors                                      | 각 (i,j)에 대해: neighbors = livingNeighbors(i,j); current = cellValueAt(i,j);                         |
| rule: dead + neighbors==3 → born                                              | if(!current && neighbors==3) next.setCellValueAt(i,j,true);                                            |
| rule: live + neighbors==2 or 3 → stays alive                                  | else if(current && (neighbors==2 || neighbors==3)) next.setCellValueAt(i,j,true);                      |
| rule: 기타는 죽음(그냥 false 유지)                                             | 아무 것도 안 함 → 기본값 false라 그대로 죽은 상태                                                     |
| nextLife returns new Life by value                                            | return next;                                                                                           |
*/
