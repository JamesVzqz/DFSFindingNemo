#include"game.h"
#include"maze.h"
#include"player.h"
#include <cstdlib>
#include<iostream>
#include <fstream>
#include <sstream>
#include<algorithm>


Game::Game(std::string settingsFile) :m_maxSteps(0), m_automate(false), m_showSteps(false){
	//std::string fsettings = "settings.ini";
	std::ifstream file(settingsFile);

	if (!file.is_open()) {
		std::cerr << "ERROR Game: could not open file " << settingsFile << ". Exiting." << std::endl;
		std::cin.ignore(); exit(0);
	}
	//Grab settings from file
	std::string line, setting, value;

	std::string mazeFile = "maze_lecture.txt";

	bool havePlayerBackTack = true; //Toggle player backtracking

	int frameTimeDelay = 500;		//Effects game speed, shorter delay
									//less time between frames, faster 
									//simulation when automating.
	int numSharks = 0;

	while (std::getline(file, line)) {
		line.erase(std::remove_if(line.begin(), line.end(),
			[](char c) { return c == ' ' || c == '\n' || c == '\r' || c == '\t'; }), line.end());
		if (line.size() == 0) continue;
		int delpos = line.find('=');
		setting = line.substr(0, delpos);
		value = line.substr(delpos + 1);

		if (setting == "mazeFile") {
			std::ifstream tf(value);

			if (!tf.is_open()) {
				std::cerr << "ERROR Game: could not open file " << value << ". Exiting." << std::endl;
				std::cin.ignore(); exit(0);
			}
			tf.close();

			mazeFile = value;
		}
		else if (setting == "havePlayerBackTack") {
			if (value != "true" && value != "false") {
				std::cerr << "ERROR Game: havePlayerBackTack setting incorrect format\n\tExpected: true/false, given: ." << value << ".\n\tExiting." << std::endl;;
				std::cin.ignore(); exit(1);
			}

			std::istringstream istr(value);
			istr >> std::boolalpha >> havePlayerBackTack;
		}
		else if (setting == "frameTimeDelay") {
			frameTimeDelay = std::stoi(value);
		}
		else if (setting == "numSharks") {
			numSharks = std::stoi(value);
		}
		else {
			std::cerr << "ERROR Game: unknown setting " << setting << " " << value << ". Exiting." << std::endl;
			std::cin.ignore(); exit(1);
		}
	}
	file.close();

	m_delay = frameTimeDelay;

	m_maze = new Maze(mazeFile, havePlayerBackTack);

	

	// Arbitrarily write maximum number of sharks the half the number of open cells
	int maxSharks = m_maze->numOpenRooms() / 2;
	if (numSharks > maxSharks) {
		std::cerr << "ERROR GAME: too many sharks: " << numSharks
			<< " for game size: " << maxSharks << ". Exiting." << std::endl;
		exit(1);
	}

	for (int i = 0; i < numSharks; i++) {
		m_maze->addBaddies();
	}

}
Game::Game(std::string filename, int numSharks, int delay, bool toggleBackTracking)
	:m_maxSteps(0), m_automate(false), m_delay(delay){

	m_maze = new Maze(filename, toggleBackTracking);

	// Arbitrarily write maximum number of sharks the half the number of open cells
	int maxSharks = m_maze->numOpenRooms() / 2;
	if (numSharks > maxSharks) {
		std::cerr << "ERROR GAME: too many sharks: "<<numSharks
			<<" for game size: " << maxSharks << ". Exiting." << std::endl;
		exit(1);
	}

	for (int i = 0; i < numSharks; i++) {
		m_maze->addBaddies();
	}

}

Game::~Game() {
	delete m_maze;
	//std::cerr << "~Game" << std::endl;
}

void Game::play(){
	std::string teleportList;
	int num_teleport = 0;
	m_showSteps = true;
	// Game loop
	//bool teleported = false;
	int teleportIter = 0;
	Point curr = m_maze->player()->room();
	Point next = curr;
	while (true) {

		// Clear the canvas to draw new scene
		clearScreen();

		m_maxSteps++;

		if (sqrDist(curr, next)>1) {
			//teleported = true;
			teleportList += "Teleporting at step: " + std::to_string(teleportIter) + "\n";
			teleportIter = m_maxSteps;
			num_teleport++;
		}

		if (m_showSteps) {
			std::cout << "Step: " << m_maxSteps
				<< ", Total Player::Update Runtime: " << m_maze->getPlayerUpdateRunTime()
				<< ", Runtime/Step: " << m_maze->getPlayerUpdateRunTime() / m_maxSteps << std::endl;
			std::cout << teleportList;
			std::cout << "Teleported: " << num_teleport << " times. " << std::endl;
			//if (teleported == true) {
			//	
				//std::cout << teleportList;
			//	teleported = false;
			//}
		}
		m_maze->draw();
		
		
		if (m_maze->player()->stuck()) {
			std::cout << "Got stuck with no way out :( " << std::endl;
			std::cout << "Press enter to continue.";
			std::cin.ignore(10000, '\n');
			return;
		}

		if (m_maze->player()->foundExit()) {
			std::cout << "You've reached the end! Congratulations! " << std::endl;
			std::cout << "Press enter to continue.";
			std::cin.ignore(10000, '\n');
			return;
		}

		

		if (!m_automate) {
			std::cout <<"Command (<space> to step, <a> to automate, <s> for stats, <q> to quit): ";
			std::string action;
			std::getline(std::cin, action);
			switch (action[0]) {
			default:
				std::cout << '\a' << std::endl;  // beep
				continue;
			case 'q':
				std::cout << "Quitting Game." << std::endl;
				return;
			case ' ':
				break;
			case 'a':
				m_automate = true;
				break;
			case 's':
				m_showSteps = !m_showSteps;
				break;
			}
		}
		else {
			if (m_maxSteps >= MAX_STEPS) {
				std::cout << "Reached max steps, quitting." << std::endl;
				return;
			}
			delay(m_delay);
		}
		
		m_maze->update();
		m_maze->interact();

		curr = next;
		next = m_maze->player()->room();
		
		
	}
} 



#include <chrono>
#include <thread>




void Game::delay(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////


// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++
#include <windows.h>

void Game::clearScreen() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void Game::clearScreen() {  // will just write a newline in an Xcode output window

	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		std::cout << std::endl;
	else {
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		std::cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << std::flush;
	}
}
#endif