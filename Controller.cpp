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
		cliente->subscribe("robot1/motor"+to_string(i)+"/voltage");
		cliente->subscribe("robot1/motor" + to_string(i) + "/current");
		cliente->subscribe("robot1/motor" + to_string(i) + "/temperature");
	}
}

/*
* Metodo que libera los recursos empleados para controlar el robot
*/
Controller::~Controller() {
	for (int i = 1; i < 5; i++)
	{
		cliente->unsubscribe("robot1/motor"+to_string(i)+"/voltage");
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

		DrawText(msj, 0, i*14, 14, WHITE);
		DrawText(msjpayload, 250, i*14, 14, WHITE);
	}
	

	if (IsKeyDown(KEY_UP))
	{
		moveForward();	
	}
	if (IsKeyReleased(KEY_UP))
	{
		frenarMotor();
	}
	if (IsKeyDown(KEY_DOWN))
	{
		moveBackward();
	}
	if (IsKeyReleased(KEY_DOWN))
	{
		frenarMotor();
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		turnRight();
	}
	if (IsKeyReleased(KEY_RIGHT))
	{
		frenarMotor();
	}
	if (IsKeyDown(KEY_LEFT))
	{
		turnLeft();
	}
	if (IsKeyReleased(KEY_LEFT))
	{
		frenarMotor();
	}
}	

/*
* Metodo que permite mover al robot hacia adelante
*/
void Controller::moveForward() {
	actualizarMotor(1, MOVE_CURRENT);
	actualizarMotor(2, -MOVE_CURRENT);
	actualizarMotor(3, 4, STOP_CURRENT);
}

/*
* Metodo que permite mover al robot hacia atras
*/
void Controller::moveBackward() {
	actualizarMotor(3, MOVE_CURRENT);
	actualizarMotor(4, -MOVE_CURRENT);
	actualizarMotor(1, 2, STOP_CURRENT);
}

/*
* Metodo que permite girar el robot hacia la derecha
*/
void Controller::turnRight() {
	actualizarMotor(1, 2, -TURN_CURRENT);
}

/*
* Metodo que permite girar al robot hacia la izquierda
*/
void Controller::turnLeft(){
	actualizarMotor(1, 2, TURN_CURRENT);

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
