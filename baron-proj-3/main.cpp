#include "ConsoleGrid.h"
#include "ConsoleInput.h"
#include "Life.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

static const string figurePath = "figures/";
static const string figures[] = {"basic-figures.txt", "burst.txt",
								 "canada-goose.txt",  "glider-gun.txt",
								 "queen-bee.txt",	  "rpent.txt"};
static const int figuresCount = sizeof(figures) / sizeof(*figures);

/*****************************************************************************/
// Utils

// return a user-selected index in the figures array
int PresentStartUI() {
	cout << "To start, specify a figure 1-" << figuresCount
		 << ", and press return:";
	cout << endl << endl;
	for (int i = 0; i < figuresCount; i++) {
		cout << i + 1 << " load " << figures[i] << endl;
	}
	int figureNumber = 0;
	do {
		cout << "> ";
		cin >> figureNumber;
	} while (figureNumber < 1 || figureNumber > figuresCount);

	return figureNumber - 1;
}

// set the grid's message to the simulation status and update
void PrintStatusMessage(ConsoleGrid *grid, long int generation, int totalLiving,
						string append = "") {
	string message =
		"Generation: " + to_string(generation) +
		"; Living cells: " + to_string(totalLiving) +
		"\nSpace bar toggles play/pause, right arrow steps, q quits.";

	if (append.length()) {
		cout << append << endl;
	}
	grid->setStatusLine(message);
}

/*****************************************************************************/

int main() {
	// prompt user for a figure index
	int figureIndex = PresentStartUI();
	const string pathFile = figurePath + figures[figureIndex];
	ifstream ifs(pathFile);
	if (!ifs.is_open()) {
		throw std::ios_base::failure("Could not open file: " + pathFile);
	}

	// instantiate Life and a ConsoleGrid with matching dimensions
	Life life(ifs);
	ifs.close();
	
	ConsoleGrid grid(life.rowCount(), life.colCount(), false);
	UserConsoleInput ci;


	// loop variables
	bool stop = false;
	bool paused = true;
	long int generation = 0;

	while (!stop) {
		// draw life state
		int totalLiving = life.draw(&grid);
		PrintStatusMessage(&grid, generation, totalLiving);

		// get user input
		int timeout = paused ? 0 : 2; // paused means wait for key indefinitely
		char cmd = ci.nextCommandKey(timeout);

		switch (cmd) {
		case 'q':
		case 'Q':
			stop = true;
			continue; // do not advance life state when quitting
			break;

		case ' ':
			if ((paused = !paused)) {
				continue; // do not advance life state when _starting_ pause
			}
			break;

		case arrowRightKey:
			paused = true;
			break;

		default:
			break;
		}
		// Please note that the following line invokes the assignment operator (=) on the life 
		// instance passing the result of nextLife().
		life = life.nextLife();
		generation++;
	}
}
