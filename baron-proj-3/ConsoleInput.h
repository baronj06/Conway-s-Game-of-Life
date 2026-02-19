#ifndef __CONSOLE_INPUT_H__
#define __CONSOLE_INPUT_H__

#include <string>
#include <fstream>

using namespace std;

enum CommandKey : char {
  noKey = 0, // we rely on this being zero, i.e. "false"

  tabKey = 0x09,
  enterKey = 0x0A,
  escapeKey = 0x1B,
  deleteKey = 0x7F,

  // Use high-bit values for special keys.
  unknownKey = char(0x80),
  arrowUpKey,
  arrowDownKey,
  arrowRightKey,
  arrowLeftKey,
  arrowUpLeftKey,
  arrowUpRightKey,
  arrowDownLeftKey,
  arrowDownRightKey,
  pageUpKey,
  pageDownKey,
  deleteForwardKey,
  commandKeyCount
};

/******************************************************************************/
/******************************************************************************/

class ConsoleInput {
public:
    ConsoleInput(string filename = "./recordedkeys.txt") : _filename(filename) {}
    virtual ~ConsoleInput() {
		if (_file.is_open()) {
			_file.close();
		}
	};
    virtual char nextCommandKey(unsigned timeout) = 0;
	virtual void writeCommandKey(char cmd) {}

	void openKeystrokeFile(ios::openmode mode) {
		_file.open(_filename, mode);
    	if (!_file.is_open()) {
        	perror(("error opening " + _filename).c_str());
    	}
	}

protected:
	fstream _file;
    string _filename;
};

/******************************************************************************/
/******************************************************************************/

class UserConsoleInput : public ConsoleInput {
public:    
    // timeout specifies how long (in tenths of seconds) to wait for user input.
    // return zero if the timeout expires.
    // timeout == 0 waits indefintiely.
    char nextCommandKey(unsigned timeout) override;

protected:
    char escapedCommandKey(char c);
	void unconditionalWriteCommandKey(char cmd);
	char nextCommandKeyHelper(unsigned timeout);
};

/******************************************************************************/
/******************************************************************************/

class RecordedConsoleInput : public ConsoleInput {
public:
	RecordedConsoleInput() : ConsoleInput() {}
    RecordedConsoleInput(string filename) : ConsoleInput(filename) {}

    // timeout specifies how long (in tenths of seconds) to wait before returning
    // timeout == 0 means use a reasonable default (0.5sec)
    char nextCommandKey(unsigned timeout) override;

private: 

};

/******************************************************************************/
/******************************************************************************/

class RobotConsoleInput : public UserConsoleInput {
public:
	char nextCommandKey(unsigned timeout) override;
	void writeCommandKey(char cmd) override;
};

#endif