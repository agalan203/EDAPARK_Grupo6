/*
 * EDAPark
 *
 * 22.08 EDA
 * TP 2
 * Santiago Michelotti y Albertina Galan
 *
 * Clases y metodos para el control del robot
 */
#include "Controller.h"

using namespace std;

/*
* Metodo que inicializa el robot
*/
Controller::Controller() {

	cliente = new MQTTClient("controller");
	
	if (!(cliente->connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez"))) 
	{
		cout << "No se logro establecer conexion" << endl;
	}
	for (int i = 1; i < 5; i++)
	{
		cliente->subscribe("robot1/motor" + to_string(i) + "/voltage");
		cliente->subscribe("robot1/motor" + to_string(i) + "/current");
		cliente->subscribe("robot1/motor" + to_string(i) + "/temperature");
	}

	bool isLEDOn = false;
}

/*
* Metodo que libera los recursos empleados para controlar el robot
*/
Controller::~Controller() {
	for (int i = 1; i < 5; i++)
	{
		cliente->unsubscribe("robot1/motor" + to_string(i) + "/voltage");
		cliente->unsubscribe("robot1/motor" + to_string(i) + "/current");
		cliente->unsubscribe("robot1/motor" + to_string(i) + "/temperature");
	}
	cliente->disconnect();
}

/*
* Metodo que permite transformar un vector de char a un float
* /https://stackoverflow.com/questions/6417438/c-convert-vectorchar-to-double
*/
float Controller::getFloatFromArray(std::vector<char> payload) {

	float convert = 0.0;
	memcpy(&convert, &payload[0], std::min(payload.size(), sizeof(float)));
	return convert;

}

/*
* Metodo que permite transformar un float a un vector de char, el formato requerido para hacer publish
* https://stackoverflow.com/questions/6417438/c-convert-vectorchar-to-double
*/
std::vector<char> Controller::getArrayFromFloat(float payload) {

	vector<char> data(sizeof(float));
	memcpy(data.data(), &payload, sizeof(float));
	return data;
}

/*
* Metodo que actualiza el estado del robot e interpreta los comandos de control
*/
void Controller::updateController() {

	vector<MQTTMessage> mensajes = cliente->getMessages();
	
	for (int i = 0; i < mensajes.size(); i++)
	{
		char* msj = const_cast<char*>(mensajes[i].topic.c_str());
		string floatpayload = to_string(getFloatFromArray(mensajes[i].payload));
		char* msjpayload = (char*)(floatpayload.c_str());

		DrawText("ROBOT1 CONTROL PANEL", 0, 0, 24, RED);
		DrawText(msj, 0, 30 + i*14, 14, WHITE);
		DrawText(msjpayload, 250, 30 + i*14, 14, WHITE);
	}
	
	if (IsKeyDown(KEY_UP))
	{
		moveRobotfront(MOVE_CURRENT);	
	}
	if (IsKeyReleased(KEY_UP))
	{
		frenarMotor();
	}
	if (IsKeyDown(KEY_DOWN))
	{
		moveRobotfront(-MOVE_CURRENT);	
	}
	if (IsKeyReleased(KEY_DOWN))
	{
		frenarMotor();
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		moveRobotside(-TURN_CURRENT);
	}
	if (IsKeyReleased(KEY_RIGHT))
	{
		frenarMotor();
	}
	if (IsKeyDown(KEY_LEFT))
	{
		moveRobotside(TURN_CURRENT);
	}
	if (IsKeyReleased(KEY_LEFT))
	{
		frenarMotor();
	}

	double time = GetTime();
	double period = time - (long)time;
	bool shouldLEDBeOn = (period < 0.1);
	if (isLEDOn != shouldLEDBeOn)
	{
		char redColor = shouldLEDBeOn ? 0xff : 0;
		vector<char> i = {redColor, 0, 0};
		cliente->publish("robot1/display/leftEye/set", i);
		cliente->publish("robot1/display/rightEye/set", i);
		isLEDOn = shouldLEDBeOn; 
	}
}	

/*
* Metodo que permite girar el robot
*/
void Controller::moveRobotfront(float current) {
	actualizarMotor(1, 4, current);
	actualizarMotor(2, 3, -current);
}

/*
* Metodo que permite mover adelante-atras el robot
*/
void Controller::moveRobotside(float current) {
	actualizarMotor(1, 2, current);
	actualizarMotor(3, 4, -current);
}

/*
* Metodos que actualizan la corriente en los motores del robot
* param n: el numero del motor a actualizar
* param current: la corriente que se le quiere dar al motor
*/
void Controller::actualizarMotor (int n1, float current){

	vector<char> i = getArrayFromFloat(current);
	cliente->publish("robot1/motor" + to_string(n1) + "/current/set", i);

}

void Controller::actualizarMotor (int n1, int n2, float current){

	vector<char> i = getArrayFromFloat(current);
	cliente->publish("robot1/motor" + to_string(n1) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n2) + "/current/set",i);

}

void Controller::actualizarMotor (int n1, int n2, int n3, int n4, float current){

	vector<char> i = getArrayFromFloat(current);
	cliente->publish("robot1/motor" + to_string(n1) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n2) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n3) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n4) + "/current/set", i);

}

/*
* Metodo que permite frenar todos los motores
*/
void Controller::frenarMotor(){

	vector<char> i = getArrayFromFloat(STOP_CURRENT);
	cliente->publish("robot1/motor" + to_string(1) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(2) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(3) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(4) + "/current/set", i);
}
