#include"shark.h"
#include"maze.h"
#include"player.h"

Shark::Shark(Maze* maze, Point p, std::string name, char sprite):
	Actor(maze, p, name, sprite){
}
void Shark::update() {

	// Pick a random direction
	int dir = maze()->randInt(0, 3);
	// Get a copy of sharks current position
	Point p  = room();

	int rows = maze()->rows();
	int cols = maze()->cols();
	int x    = p.readX();
	int y    = p.readY();

	// Accounting for the Walls and one space to allow
	// movement in that direction. Update the x,y
	// coordinates or do nothing
	switch (dir) {
	case 0:  if (y <= 2)      return; else y--; break;
	case 1:  if (y >= rows-3) return; else y++; break;
	case 2:  if (x <= 2)      return; else x--; break;
	case 3:  if (x >= cols-3) return; else x++; break;
	default: return;
	}

	// Check to see if new position is an open cell
	p.write(x, y);
	if (!maze()->open(p)) 
		return;

	move(p);
}

void Shark::say() {
	switch (readInteract()) {
	case Interact::ATTACK:
		std::cout << getName() << ": OM NOM!";
		break;
	case Interact::ALONE:
		std::cout << getName() << ": I'm hungry";
		break;
	case Interact::GREET:
		std::cout << getName() << ": Hey Buddy";
		break;
	default:
		std::cout << getName() << ":A - hee - ahee ha - hee!";
	}
}