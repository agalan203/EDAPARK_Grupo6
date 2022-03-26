/*
 * EDAPark
 *
 * 22.08 EDA
 * TP 2
 * Santiago Michelotti y Albertina Galan
 *
 * Clases y metodos para el control del robot
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "MQTTClient.h"
#include "raylib.h"
#include <iostream>
#include <string.h>
#include <vector>

#define MOVE_CURRENT 6.0F
#define STOP_CURRENT 0.0F
#define TURN_CURRENT 0.5F

/*
* Clase que permite inicializar y controlar el robot
*/
class Controller
{
public:
	/*
	* Metodo que inicializa el robot
	*/
	Controller();
	/*
	* Metodo que libera los recursos empleados para controlar el robot
	*/
	~Controller();

	/*
	* Metodo que actualiza el estado del robot e interpreta los comandos de control
	*/
	void updateController();

	MQTTClient* cliente;	
	
private:

	bool isLEDOn;

	/*
	* Metodo que permite mover el robot hacia los costados
	*/
	void moveRobotside(float current);
	/*
	* Metodo que permite mover adelante-atras el robot
	*/
	void moveRobotfront(float current);
	/*
	* Metodo que permite girar al robot
	*/
	void turnRobot(void);

	/*
	* Metodos que actualizan la corriente en los motores del robot
	* param n: el numero del motor a actualizar
	* param current: la corriente que se le quiere dar al motor
	*/
	void actualizarMotor (int n1, float current);
	void actualizarMotor (int n1, int n2, float current);
	void actualizarMotor (int n1, int n2, int n3, int n4, float current);

	/*
	* Metodo que permite frenar todos los motores
	*/
	void frenarMotor (void);

	/*
	* Metodo que permite transformar un vector de char a un float
	* https://stackoverflow.com/questions/6417438/c-convert-vectorchar-to-double
	*/
	float getFloatFromArray (std::vector<char> payload);
	/*
	* Metodo que permite transformar un float a un vector de char, el formato requerido para hacer publish
	* https://stackoverflow.com/questions/6417438/c-convert-vectorchar-to-double
	*/
	std::vector<char> getArrayFromFloat(float payload);

	void DrawTable();
};

#endif // !CONTROLLER_H
