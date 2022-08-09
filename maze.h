#ifndef WORLD_H
#define WORLD_H

#include"dllist.h"
#include"point.h"


const int MAX_ACTORS = 50;
class Player;
class Actor;

class Maze {
public:
	// Constructor: create the maze from file.
	// file must not have any white space after last
	// col and must have one newline after last row.
	// Creates the player, sets the starting and end 
	// points
	Maze(std::string filename, const bool tbacktrack);
	~Maze();

	int		rows() const;	// Get the size of the maze
	int		cols() const;

	int		numOpenRooms() const;
	bool	open(Point p) const;

	bool	foundExit(Point p) const;

	Player*	player() const;	// Get the player from the maze
	int		addBaddies();

	double  getPlayerUpdateRunTime(); //Time it takes for the player to make their move

	void	update();		// Update the maze by one step
	void	interact();		// Update the Actors' interactions
	void	draw() const;	// Draw the maze with actors

	int randInt(int min, int max);
private:
	

	DLList<Point> m_openRooms;	// Store all the cells actors can move to

	int		m_rows;				// Extents of the maze
	int		m_cols;

	Point	m_start;			// Start of the maze
	Point	m_end;				// End point of the maze

	char**	m_tmaze;			// maze template created from file
	char**	m_renderedMaze;	    // Copy of maze with actors drawn

	Player*	m_player;			// Dynamically allocated player

	Actor*	m_actors[MAX_ACTORS]; // Polymorphic array of actors
								  // The player is always the 0th element
	int		m_nActors;

	double  m_duration;		// How long Player::update takes to execute

	
};

#endif//WORLD_H