#ifndef ACTOR_H
#define ACTOR_H

#include"point.h"
#include<string>

// An Actor can have certain States that describe 
// What they are doing
enum class State { LOOKING, NOEXIT, BACKTRACKING, FOUNDEXIT };

// Interactions between Actors
enum class Interact { GREET, ATTACK, ALONE };

class Maze;

class Actor {
public:

	Actor(Maze* maze, Point p, std::string, char sprite);
	virtual ~Actor();
	// Pure virtual functions: Actor is an ABC
	// Derived classes must implement
	virtual	void	update() = 0;
	virtual	void	say() = 0;

	// Maybe overriden in derived class
	// Otherwise just returns the char
	// representing the Actor
	virtual	char	draw() const;

	// Setters and Getters for common attributes
	std::string		getName();

	void			move(Point p);
	Point			room() const;

	void			writeState(State s);
	State			readState() const;

	void			writeInteract(Interact i);
	Interact		readInteract() const;

	Maze*			maze() const;

private:
	Point		m_currPos;		//Current position

	State		m_state;
	Interact    m_interact;

	char		m_sprite;
	Maze*		m_maze;		//Pointer to the maze
							//Actor does not allocate
							//Just points to it.
	std::string m_name;
};

#endif//ACTOR_H