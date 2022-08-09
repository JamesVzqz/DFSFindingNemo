#include"actor.h"
#include"point.h"
#include"maze.h"
#include<string>							
Actor::Actor(Maze* maze, Point p, std::string name, char sprite)
	:m_maze(maze), m_currPos(p),m_name(name),
	m_sprite(sprite), m_state(State::LOOKING), m_interact(Interact::ALONE) {
}
Actor::~Actor() {}
void Actor::move(Point p) {
	m_currPos = p;
}
Point Actor::room() const {
	return m_currPos;
}
void Actor::writeState(State s) {
	m_state = s;
}
State  Actor::readState() const {
	return m_state;
}
char Actor::draw() const {
	return m_sprite;
}
Maze* Actor::maze() const {
	return m_maze;
}
std::string Actor::getName() {
	return m_name;
}

void Actor::writeInteract(Interact i) {
	m_interact = i;
}
Interact Actor::readInteract() const {
	return m_interact;
}