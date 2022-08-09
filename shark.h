#ifndef SHARK_H
#define SHARK_H
#include"actor.h"
// Shark NPC's that roam the maze and bite at Nemo
class Shark : public Actor {
public:
	Shark(Maze* maze, Point p, std::string name, char sprite);
	virtual void update();	// Sharks move randomly in the world in search of prey
	virtual void say();		// What does the shark say? 
private:
};

#endif//SHARK_H