#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "MQTTClient.h"
#include "raylib.h"
#include <iostream>
#include <string.h>
#include <vector>

#define MOVE_CURRENT 8.0F
#define STOP_CURRENT 0.0F
#define TURN_CURRENT 1.0F

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

	MQTTClient* cliente;	

private:

	void actualizarMotor (int n1, float current);
	void actualizarMotor (int n1, int n2, float current);
	void actualizarMotor (int n1, int n2, int n3, int n4, float current);
	void frenarMotor (void);

	float getFloatFromArray (std::vector<char> payload);
	std::vector<char> getArrayFromFloat(float payload);
};

#endif // !CONTROLLER_H
