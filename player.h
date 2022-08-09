#ifndef PLAYER_H_
#define PLAYER_H_

#include"actor.h"
#include"queue.h"
#include"stack.h"
#include"dllist.h"

class Point;
class Maze;

class Player :public Actor {
public:
	Player(Maze* maze, Point p, std::string name, char sprite, const bool tbacktrack);

	virtual void	update(); // The only function of player to implement


	virtual void	say();

	bool			foundExit() const;
	bool			stuck() const;
	Point			getTargetPoint() const;

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif

	// Wrapper function to find if a point has been
	// placed inside the m_discoveredRooms List
	bool discovered(const Point &p) const;
	
	// Stack to decide where to look next
	Stack<Point>	m_lookingPaper;

	// List to save all the discovered areas, 
	// Helps us decide where to look next
	DLList<Point>		m_discoveredRooms;

	// ** Variables relating to backtracking. **

	// Read from settings.ini file, write at player creation
	// and cannot be modified, only read.
	// All back tracking behavior should be toggled using
	// m_backtrackingEnabled, if false the player teleports
	// the way seen in the algorithm from the lecture slides.
	const bool		m_backtrackingEnabled;

	// Used to facilitate backtracking, without backtracking, 
	// the player teleports to points for looking.  
	//
	// To simulate real movement we need to interpolate between
	// The players current position to the next point to look.
	//
	// The premise is that if there is a point to look around, 
	// The player must have been next to that point earlier (by
	// definition of the algorithm) so if we keep track of where
	// the player has been we can back track our steps to reach
	// the desired point.
	//
	Stack<Point>	m_btStack;
	Queue<Point>	m_btQueue;

};

#endif//PLAYER_H_