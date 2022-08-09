#include"player.h"
#include"actor.h"
#include"stack.h"
#include"dllist.h"
#include"point.h"
#include"maze.h"
#include<iostream>

//  Player( ... )
//		Constructs and initializes the Player/Actor and its member variables
//		Remembers and discovers the starting point.
Player::Player(Maze* maze, Point p, std::string name, char sprite, const bool tbacktrack)
	:Actor(maze, p, name, sprite),
	m_lookingPaper(),
	m_discoveredRooms(),
	m_btQueue(),
	m_btStack(),
	m_backtrackingEnabled(tbacktrack) {

	// Discover the starting point
	m_discoveredRooms.push_front(p);
	m_lookingPaper.push(p);
}

//  stuck() 
//		See if the player is stuck in the maze (no solution)
bool Player::stuck() const {
	return Actor::readState() == State::NOEXIT;
}

//  foundExit()
//		See if the player has found the exit
bool Player::foundExit() const {
	return Actor::readState() == State::FOUNDEXIT;
}

//  getTargetPoint()
//		Get the point the player wants to look around next.
//      If m_lookingPaper is empty then return an invalid point.
Point Player::getTargetPoint() const {
	if (m_lookingPaper.empty()) return Point(-1, -1);
	return m_lookingPaper.peek();
}

// discovered
//		returns true if the item is in the list
bool Player::discovered(const Point &p) const {
	return (m_discoveredRooms.find(p) != -1);
}



// say()
//		What does the player say?
void Player::say() {

	// Freedom supercedes being eaten
	if (readState() == State::FOUNDEXIT) {
		std::cout << getName() << ": WEEEEEEEEE!";
		return;
	} 
 
	// Being eaten supercedes being lost
	switch (readInteract()) {
	case Interact::ATTACK:
		std::cout << getName() << ": OUCH!";
		break;
	case Interact::GREET:
		break;
	case Interact::ALONE:
	default:
		switch (readState()) {
		case State::LOOKING:
			std::cout << getName() << ": Where is the exit?";
			break;
		case State::NOEXIT:
			std::cout << getName() << ": Oh no! I am Trapped!";
			break;
		case State::BACKTRACKING:
			std::cout << getName() << ": Got to backtrack...";
			break;
		default:
			break;
		}

		break;
	}
}


//  update() 
//		This function implements an algorithm to look through the maze
//      for places to move (STATE::LOOKING). update also handles moving the 
//		player as well, if there is an open undiscovered cell to move to the  
//		player just moves there.  However, there will be cases in the next 
//		desired point to look around is not adjacent (more than one cell away), 
//		at this point the player must backtrack (State::BACKTRACKING) to a point 
//		that is adjacent to the next point the player was planning to visit
//
//		The player can only do one thing each call, they can either LOOKING or 
//		BACKTRACKING not both.  Nor should the player move more than one cell per
//		call to update.  If you examine the function calls for the entire game
//		you should observe that this is already be called within a loop.
//
void Player::update() {

	// Set by the settings file, if m_backtrackingEnabled is false, then
	// your program should behave exactly as seen in the slides or
	// example executables (with teleporting).
	// if(m_backtrackingEnabled) { ... code relating to backtracking 

	if (m_lookingPaper.empty()) {
		Actor::writeState(State::NOEXIT);
		return;
	}

	Point next = m_lookingPaper.peek();


	if (m_backtrackingEnabled) {
		Point curr = room();

		double dist = sqrDist(curr, next);
		if (m_btStack.empty() || dist <= 1.0) {
			writeState(State::LOOKING);
		}
		else {
			writeState(State::BACKTRACKING);
			move(m_btStack.peek());
			m_btStack.pop();
			return;
		}
		m_btStack.push(curr);
	}


	m_lookingPaper.pop();
	Actor::move(next);
	Maze *tmp = maze();
	if (tmp->foundExit(next)) {
		Actor::writeState(State::FOUNDEXIT);
		return;
	}

	// Check West 
	Point west(next.readX() - 1, next.readY());
	if (tmp->open(west) && !discovered(west)) {
		m_lookingPaper.push(west);
		m_discoveredRooms.push_front(west);
	}
	Point east(next.readX() + 1, next.readY());
	if (tmp->open(east) && !discovered(east)) {
		m_lookingPaper.push(east);
		m_discoveredRooms.push_front(east);
	}
	Point north(next.readX(), next.readY() - 1);
	if (tmp->open(north) && !discovered(north)) {
		m_lookingPaper.push(north);
		m_discoveredRooms.push_front(north);
	}
	Point south(next.readX(), next.readY() + 1);
	if (tmp->open(south) && !discovered(south)) {
		m_lookingPaper.push(south);
		m_discoveredRooms.push_front(south);
	}
}