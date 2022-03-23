#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "MQTTClient.h"
#include "raylib.h"
#include <iostream>

#define DIAGONAL_CUAD1 0
#define DIAGONAL_CUAD2 1
#define DIAGONAL_CUAD3 2
#define DIAGONAL_CUAD4 3

class Controller
{
public:
	Controller();
	~Controller();
	void updateController();
	void moveForward();
	void moveBackward();
	void turnRight();
	void turnLeft();
	void moveDiagonal(int diagonal);
	MQTTClient* cliente;
	

private:
};

#endif // !CONTROLLER_H
