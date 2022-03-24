#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "MQTTClient.h"
#include "raylib.h"
#include <iostream>
#include <string.h>
#include <vector>

#define DIAGONAL_CUAD1 1
#define DIAGONAL_CUAD2 2
#define DIAGONAL_CUAD3 3
#define DIAGONAL_CUAD4 4

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
	void motoresHorario(int n1);
	void motoresHorario(int n1, int n2);
	void motoresHorario(int n1, int n2,int n3, int n4);
	void motoresAntiHorario(int n1);
	void motoresAntiHorario(int n1, int n2);
	void motoresAntiHorario(int n1, int n2m, int n3, int n4);
	void motoresDetenidos(int n1, int n2);
	void motoresDetenidos(int n1, int n2, int n3, int n4);

	float getFloatFromArray (std::vector<char> payload);
	std::vector<char> getArrayFromFloat(float payload);
};

#endif // !CONTROLLER_H
