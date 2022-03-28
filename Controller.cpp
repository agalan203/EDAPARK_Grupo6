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
			float floatpayload = getFloatFromArray(mensajes[i].payload);
			string stringpayload = to_string(floatpayload);
			char* msjpayload = (char*)(stringpayload.c_str());
			
			if (mensajes[i].topic.find("1/voltage"))
			{
				if(floatpayload >= 24.0F || floatpayload <= -24.0F)
				{
					DrawText("WARNING", 125 + i * 5000, 125 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 125 + i * 5000, 125 + i * 5000, 20, WHITE);
				}
			}
			if (mensajes[i].topic.find("2/voltage"))
			{
				if(floatpayload >= 24.0F || floatpayload <= -24.0F)
				{
					DrawText("WARNING", 125 + i * 5000, 185 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 125 + i * 5000, 185 + i * 5000, 20, WHITE);
				}
			}
			if (mensajes[i].topic.find("3/voltage"))
			{
				if(floatpayload >= 24.0F || floatpayload <= -24.0F)
				{
					DrawText("WARNING", 125 + i * 5000, 245 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 125 + i * 5000, 245 + i * 5000, 20, WHITE);
				}
			}
			if (mensajes[i].topic.find("4/voltage"))
			{
				if(floatpayload >= 24.0F || floatpayload <= -24.0F)
				{
					DrawText("WARNING", 125 + i * 5000, 305 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 125 + i * 5000, 305 + i * 5000, 20, WHITE);
				}
			}

			if (mensajes[i].topic.find("1/current"))
			{
				if(floatpayload >= 10.0F || floatpayload <= -10.0F)
				{
					DrawText("WARNING", 270 + i * 5000, 125 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 270 + i * 5000, 125 + i * 5000, 20, WHITE);
				}
			}
				
			if (mensajes[i].topic.find("2/current"))
			{
				if(floatpayload >= 10.0F || floatpayload <= -10.0F)
				{
					DrawText("WARNING", 270 + i * 5000, 185 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 270 + i * 5000, 185 + i * 5000, 20, WHITE);
				}
			}
			if (mensajes[i].topic.find("3/current"))
			{
				if(floatpayload >= 10.0F || floatpayload <= -10.0F)
				{
					DrawText("WARNING", 270 + i * 5000, 245 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 270 + i * 5000, 245 + i * 5000, 20, WHITE);
				}
			}
			if (mensajes[i].topic.find("4/current"))
			{
				if(floatpayload >= 10.0F || floatpayload <= -10.0F)
				{
					DrawText("WARNING", 270 + i * 5000, 305 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 270 + i * 5000, 305 + i * 5000, 20, WHITE);
				}	
			}

			if (mensajes[i].topic.find("1/temperature"))
			{
				if(floatpayload >= 35.0F)
				{
					DrawText("WARNING", 420 + i * 5000, 125 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 420 + i * 5000, 125 + i * 5000, 20, WHITE);
				}
			}
			if (mensajes[i].topic.find("2/temperature"))
			{
				if(floatpayload >= 35.0F)
				{
					DrawText("WARNING", 420 + i * 5000, 185 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 420 + i * 5000, 185 + i * 5000, 20, WHITE);
				}
			}
			if (mensajes[i].topic.find("3/temperature"))
			{
				if(floatpayload >= 35.0F)
				{
					DrawText("WARNING", 420 + i * 5000, 245 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 420 + i * 5000, 245 + i * 5000, 20, WHITE);
				}
			}
			if (mensajes[i].topic.find("4/temperature"))
			{
				if(floatpayload >= 35.0F)
				{
					DrawText("WARNING", 420 + i * 5000, 305 + i * 5000, 20, WHITE);
				}
				else
				{
					DrawText(msjpayload, 420 + i * 5000, 305 + i * 5000, 20, WHITE);
				}
			}
		}

	//para gamepad usaria las funciones IsGamepadbuttondown en vez 
	if (IsKeyDown(KEY_UP))	//IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)
	{
		moveRobot(MOVE_CURRENT, 0);	
	}
	if (IsKeyReleased(KEY_UP))	//IsGamepadButtonUp(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)
	{
		moveRobot(STOP_CURRENT, 0);
	}
	if (IsKeyDown(KEY_DOWN))	//IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)
	{
		moveRobot(-MOVE_CURRENT, 0);	
	}
	if (IsKeyReleased(KEY_DOWN))	//IsGamepadButtonUp(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)
	{
		moveRobot(STOP_CURRENT, 0);
	}
	if (IsKeyDown(KEY_RIGHT))	//IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)
	{
		moveRobot(MOVE_CURRENT, 1);
	}
	if (IsKeyReleased(KEY_RIGHT))	//IsGamepadButtonUp(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)
	{
		moveRobot(STOP_CURRENT, 0);
	}
	if (IsKeyDown(KEY_LEFT))	//IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)
	{
		moveRobot(-MOVE_CURRENT, 1);
	}
	if (IsKeyReleased(KEY_LEFT))	//IsGamepadButtonUp(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)
	{
		moveRobot(STOP_CURRENT, 0);
	}
	if (IsKeyDown(KEY_SPACE))	//IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)
	{
		turnRobot();
	}
	if (IsKeyReleased(KEY_SPACE))	//IsGamepadButtonUp(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)
	{
		moveRobot(STOP_CURRENT, 0);
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
* Metodo que permite girar al robot
*/
void Controller::turnRobot(){
	actualizarMotor(1, -TURN_CURRENT);
	actualizarMotor(2, -TURN_CURRENT);
}

/*
* Metodo que permite mover el robot
* 1 si es Left-right, 0 si es Up-down
* current + si es arriba-derecha, - si es abajo-izquierda
*/
void Controller::moveRobot(float current, bool situation){

	actualizarMotor(1, situation==true? -current : current );
	actualizarMotor(2, -current);
	actualizarMotor(3, situation==true? current : -current );
	actualizarMotor(4, current);

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

/*
* Metodo que permite dibujar la tabla
*/
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
	DrawText("Voltage", 125, 65, 25, WHITE);
	DrawText("Current", 260, 65, 25, WHITE);
	DrawText("Temperature", 400, 65, 25, WHITE);
}