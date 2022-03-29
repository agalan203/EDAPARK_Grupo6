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

#define MOVE_CURRENT 2.0F

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

	MQTTClient *cliente;

private:
	/*
	 * Control para el parpadeo de los ojos del robot
	 */
	bool isLEDOn;

	/*
	 * Metodo que permite mover el robot
	 */
	void moveRobot(void);

	/*
	 * Metodo que actualiza la corriente en los motores del robot
	 * param n: el numero del motor a actualizar
	 * param current: la corriente que se le quiere dar al motor
	 */
	void actualizarMotor(int n, float current);

	/*
	 * Metodo que permite transformar un vector de char a un float
	 * https://stackoverflow.com/questions/6417438/c-convert-vectorchar-to-double
	 * param: vector de char con el payload del mensaje
	 * return: la conversion a float del vector
	 */
	float getFloatFromArray(std::vector<char> payload);
	/*
	 * Metodo que permite transformar un float a un vector de char, el formato requerido para hacer publish
	 * https://stackoverflow.com/questions/6417438/c-convert-vectorchar-to-double
	 * param: float para enviar como mensaje
	 * return: la conversion a vector char del numero
	 */
	std::vector<char> getArrayFromFloat(float payload);

	/*
	 * Metodo que permite dibujar la tabla
	 */
	void DrawTable(void);
};

#endif // !CONTROLLER_H
