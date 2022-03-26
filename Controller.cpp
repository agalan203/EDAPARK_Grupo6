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

	// para utilizar un gamepad/joystick, aca pondria llamadas a IsGamepadAvailable(int gamepad) y GetGamepadName(int gamepad);   

	for (int i = 1; i < 5; i++)
	{
		cliente->subscribe("robot1/motor" + to_string(i) + "/voltage");
		cliente->subscribe("robot1/motor" + to_string(i) + "/current");
		cliente->subscribe("robot1/motor" + to_string(i) + "/temperature");
	}

	bool isLEDOn = false;
	timeSinceLast = 0;
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
	DrawTable();
	DrawText("ROBOT1 CONTROL PANEL", 80, 0, 35, RED);

		for (int i = 0; i < mensajes.size(); i++)
		{
			char* msj = const_cast<char*>(mensajes[i].topic.c_str());
			string floatpayload = to_string(getFloatFromArray(mensajes[i].payload));
			char* msjpayload = (char*)(floatpayload.c_str());
			// /* sacar las // para borrar los datos en forma de tabla
			
			if (mensajes[i].topic.find("1/voltage"))
				DrawText(msjpayload, 125 + i * 5000, 125 + i * 5000, 20, WHITE);
			if (mensajes[i].topic.find("2/voltage"))
				DrawText(msjpayload, 125 + i * 5000, 185 + i * 5000, 20, WHITE);
			if (mensajes[i].topic.find("3/voltage"))
				DrawText(msjpayload, 125 + i * 5000, 245 + i * 5000, 20, WHITE);
			if (mensajes[i].topic.find("4/voltage"))
				DrawText(msjpayload, 125 + i * 5000, 305 + i * 5000, 20, WHITE);
			if (mensajes[i].topic.find("1/current"))
				DrawText(msjpayload, 270 + i * 5000, 125 + i * 5000, 20, WHITE);
			if (mensajes[i].topic.find("2/current"))
				DrawText(msjpayload, 270 + i * 5000, 185 + i * 5000, 20, WHITE);
			if (mensajes[i].topic.find("3/current"))
				DrawText(msjpayload, 270 + i * 5000, 245 + i * 5000, 20, WHITE);
			if (mensajes[i].topic.find("4/current"))
				DrawText(msjpayload, 270 + i * 5000, 305 + i * 5000, 20, WHITE);
			if (mensajes[i].topic.find("1/temperature"))
				DrawText(msjpayload, 420 + i * 5000, 125 + i * 5000, 20, WHITE);
			if (mensajes[i].topic.find("2/temperature"))
				DrawText(msjpayload, 420 + i * 5000, 185 + i * 5000, 20, WHITE);
			if (mensajes[i].topic.find("3/temperature"))
				DrawText(msjpayload, 420 + i * 5000, 245 + i * 5000, 20, WHITE);
			if (mensajes[i].topic.find("4/temperature"))
				DrawText(msjpayload, 420+i*5000, 305+i*5000, 20, WHITE); // */
			//DrawText(msj, 0, 30 + i*14, 14, WHITE);
			//DrawText(msjpayload, 250+3*i, 30 + i*12, 14, WHITE);
		}

	//para joystick usaria las funciones IsGamepadbuttondown en vez 
	if (IsKeyDown(KEY_UP))	//IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)
	{
		moveRobotfront(MOVE_CURRENT);	
	}
	if (IsKeyReleased(KEY_UP))	//IsGamepadButtonUp(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)
	{
		frenarMotor();
	}
	if (IsKeyDown(KEY_DOWN))	//IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)
	{
		moveRobotfront(-MOVE_CURRENT);	
	}
	if (IsKeyReleased(KEY_DOWN))	//IsGamepadButtonUp(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)
	{
		frenarMotor();
	}
	if (IsKeyDown(KEY_RIGHT))	//IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)
	{
		moveRobotside(-TURN_CURRENT);
	}
	if (IsKeyReleased(KEY_RIGHT))	//IsGamepadButtonUp(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)
	{
		frenarMotor();
	}
	if (IsKeyDown(KEY_LEFT))	//IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)
	{
		moveRobotside(TURN_CURRENT);
	}
	if (IsKeyReleased(KEY_LEFT))	//IsGamepadButtonUp(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)
	{
		frenarMotor();
	}
	if (IsKeyDown(KEY_SPACE))	//IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)
	{
		turnRobot();
	}
	if (IsKeyReleased(KEY_SPACE))	//IsGamepadButtonUp(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)
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
* Metodo que permite girar al robot
*/
void Controller::turnRobot(){
	actualizarMotor(1, 2, -TURN_CURRENT);

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

void Controller::DrawTable() {
	DrawLine(40, 100, 540, 100, WHITE);
	DrawLine(40, 160, 540, 160, WHITE);
	DrawLine(40, 220, 540, 220, WHITE);
	DrawLine(40, 280, 540, 280, WHITE);
	DrawLine(95, 40, 95, 340, WHITE);
	DrawLine(245, 40, 245, 340, WHITE);
	DrawLine(395, 40, 395, 340, WHITE);

	DrawText("Nro", 40, 65, 25, WHITE);
	DrawText("1", 55, 125, 25, WHITE);
	DrawText("2", 55, 185, 25, WHITE);
	DrawText("3", 55, 245, 25, WHITE);
	DrawText("4", 55, 305, 25, WHITE);
	DrawText("Voltaje", 125, 65, 25, WHITE);
	DrawText("Corriente", 260, 65, 25, WHITE);
	DrawText("Temperatura", 400, 65, 25, WHITE);
}