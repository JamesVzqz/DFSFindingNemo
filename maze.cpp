
#include"maze.h"
#include"player.h"
#include"shark.h"
#include<iostream>
#include <fstream>
#include <iterator>
#include<string>
#include <chrono>
Maze::Maze(std::string filename, const bool tbacktrack):
	m_openRooms(), m_nActors(0), m_duration(0){

	std::fstream file(filename);

	//assert(file.is_open());
	if (!file.is_open()) { 
		std::cerr << "ERROR Maze: could not open file " <<filename<<". Exiting." << std::endl;
		exit(1);
	}
	int col = 0, row = 0;
	// Count rows and columns in file
	for (std::istreambuf_iterator<char> iter(file), e; iter != e; ++iter) {
		char c = *iter;
		if (!(c == '\n' || c == '\r'))
			col++;
		if (c == '\n') {
			row++;
		}

	}
	// columns size occured row-times, divide out
	m_cols = (int)(col / row);
	m_rows = row;
	
	// Allocate the maze 
	m_tmaze = new char*[m_cols];
	m_renderedMaze = new char*[m_cols];
	for (int i = 0; i < m_cols; i++) {
		m_tmaze[i] = new char[m_rows];
		m_renderedMaze[i] = new char[m_rows];
	}
	int x = 0, y = 0;
	// Reset file 
	file.clear();
	file.seekg(0, std::ios::beg);
	// Fill in the maze
	for (std::istreambuf_iterator<char> iter(file), e; iter != e; ++iter) {
		char c = *iter;
		if (!(c == '\n' || c == '\r')) {
			m_tmaze[(x++) % m_cols][y] = c;
		}
		if (c == '\n') {
			y++;
		}
	}
	file.close();
	// Store number of free cells
	for (int y = 0; y < m_rows; y++) {
		for (int x = 0; x < m_cols; x++) {
			if (m_tmaze[x][y] != 'X' && m_tmaze[x][y] != 'S')
				m_openRooms.push_front(Point(x, y));
			if (m_tmaze[x][y] == 'S') {
				m_tmaze[x][y] = ' ';
				m_start.write(x, y); 
			}
			if (m_tmaze[x][y] == 'E') {
				m_tmaze[x][y] = ' ';
				m_end.write(x, y);
			}
		}
	}
	
	m_player = new Player(this, m_start,"Nemo", '@', tbacktrack);
	// 0th actor Polymorphic handle to player
	m_actors[m_nActors++] = m_player;
}

Maze::~Maze() {
	for (int i = 0; i < m_cols; i++) {
		delete[] m_renderedMaze[i];
		delete[] m_tmaze[i];
		
	}
	delete[] m_tmaze;

	delete[] m_renderedMaze;

	for (int i = 0; i < m_nActors; i++) {
		delete m_actors[i];
	}
}



void Maze::update() {

	auto t1 = std::chrono::high_resolution_clock::now();
	m_actors[0]->update();
	auto t2 = std::chrono::high_resolution_clock::now();
	m_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

	for (int i = 1; i < m_nActors; i++) {
		m_actors[i]->update();
	}
}

double Maze::getPlayerUpdateRunTime() {
	return m_duration;
}
void Maze::interact() {

	// Reset Interactions
	for (int i = 0; i < m_nActors; i++)
		m_actors[i]->writeInteract(Interact::ALONE);

	// Permute over Actors
	// If actors are on top of eachother, have them interact
	for (int i = 0; i < m_nActors - 1; i++) {
		Point pos = m_actors[i]->room();
		for (int j = i + 1; j < m_nActors; j++) { // j never 0
			if (pos == m_actors[j]->room()) {
				// Actor 0 is nemo, any interaction between nemo and anything
				// else is an ATTACK
				if (i == 0) {
					m_actors[i]->writeInteract(Interact::ATTACK);
					m_actors[j]->writeInteract(Interact::ATTACK);
				}
				else { 
					// If the sharks are attacking that means nemo is in the same cell
					// as multiple sharks. Attacking takes precedence over greeting
					if (m_actors[i]->readInteract() != Interact::ATTACK) {
						m_actors[i]->writeInteract(Interact::GREET);
						m_actors[j]->writeInteract(Interact::GREET);
					}
				}
			}
		}
	}
}
void Maze::draw() const {
	
	

	// Copy the clean version of the maze into the rendered version
	for (int y = 0; y < m_cols; y++) {
		memcpy(m_renderedMaze[y], m_tmaze[y], m_rows * sizeof(char));
	}
	
	// Encode the output, go through all actors and place them in the maze
	// Nemo: '@', Sharks: 'S', Multiple Shars: '2'-'9', An Attack '!'
	for (int i = 0; i < m_nActors; i++) {
		Point pos = m_actors[i]->room();
		// Reference to the the character at this position, for convenience
		char& c = m_renderedMaze[pos.readX()][pos.readY()];
		switch (c) {
		case ' ': c = m_actors[i]->draw(); break; // Empty cell, just draw
		case '@': c = '!'; break;	// ATTACK
		case 'S': c = '2'; break;	// Two sharks
		case '!':  break;			// MORE ATTACK. do nothing, already ded..
		case '9':  break;			// do nothing
		default: c++; break;		// '2' through '8', a party
		}	
	}
	// Render the next point to visit for looking
	
	if (!(m_player->stuck() || m_player->foundExit())) {
		Point target = m_player->getTargetPoint();
		if(target.readX()>=0 && target.readY() >= 0)
			m_renderedMaze[target.readX()][target.readY()] = 'T';
	}
		
	
	// Draw maze with actors
	for (int y = 0; y < m_rows; y++) {
		for (int x = 0; x < m_cols; x++) {
			std::cout << m_renderedMaze[x][y];
		}
		std::cout << std::endl;
	}
	std::cout <<  "===" << std::endl;
	
	// Display actors Interactions
	for (int i = 0; i < m_nActors; i++) {
		m_actors[i]->say();
		std::cout << std::endl;
	}
}

int	Maze::rows() const {
	return m_rows;
}
int	Maze::cols() const {
	return m_cols;
}
int	Maze::numOpenRooms() const {
	return m_openRooms.count();
}
Player*	Maze::player() const {
	return m_player;
}

bool Maze::foundExit(Point p) const {
	return p == m_end;
}
bool Maze::open(Point p) const {
	return (m_openRooms.find(p) != -1);
}
int Maze::addBaddies() {

	// Randomly place sharks
	Point p;
	do {
		int index = randInt(0, m_openRooms.count() - 1);
		p = m_openRooms.at(index);
	
	} while ((p == m_player->room())); // Don't start where the player starts

	std::string name = "Shark " + std::to_string((m_nActors - 1));
	m_actors[m_nActors++] = new Shark(this, p, name, 'S');
	return m_nActors;
}

#include <random>
#include <utility>
// Return an uniformly distributed random int from min to max, inclusive
int Maze::randInt(int min, int max)
{
	if (max < min)
		std::swap(max, min);
	static std::random_device rd;
	static std::mt19937 generator(rd());
	std::uniform_int_distribution<> distro(min, max);
	return distro(generator);
}