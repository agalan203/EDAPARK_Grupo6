/*
 * EDAPark
 *
 * 22.08 EDA
 * TP 2
 * Santiago Michelotti y Albertina Galan
 *
 * Clases y metodos para el control del robot
 * Disclaimer: no tenemos un gamepad asi que no pudimos probar si funciona,
 * pero se incluyeron las funciones
 */
#include "Controller.h"

using namespace std;

/*
 * Metodo que inicializa el robot
 */
Controller::Controller()
{
	cliente = new MQTTClient("controller");

	if (!(cliente->connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez")))
	{
		cout << "No se logro establecer conexion" << endl;
	}

	// para utilizar un gamepad/joystick
	if (IsGamepadAvailable(0))
	{
		DrawText("Gamepad Connected: use arrows to move, triggers to turn", 50, 385, 14, WHITE);
	}

	for (int i = 1; i < 5; i++)
	{
		cliente->subscribe("robot1/motor" + to_string(i) + "/voltage");
		cliente->subscribe("robot1/motor" + to_string(i) + "/current");
		cliente->subscribe("robot1/motor" + to_string(i) + "/temperature");
	}

	isLEDOn = false;
}

/*
 * Metodo que libera los recursos empleados para controlar el robot
 */
Controller::~Controller()
{
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
 * https://stackoverflow.com/questions/6417438/c-convert-vectorchar-to-double
 * param: vector de char con el payload del mensaje
 * return: la conversion a float del vector
 */
float Controller::getFloatFromVector(std::vector<char> payload)
{
	float convert = 0.0;
	memcpy(&convert, &payload[0], std::min(payload.size(), sizeof(float)));
	return convert;
}

/*
 * Metodo que permite transformar un float a un vector de char, el formato requerido para publish
 * https://stackoverflow.com/questions/6417438/c-convert-vectorchar-to-double
 * param: float para enviar como mensaje
 * return: la conversion a vector char del numero
 */
std::vector<char> Controller::getVectorFromFloat(float payload)
{
	vector<char> data(sizeof(float));
	memcpy(data.data(), &payload, sizeof(float));
	return data;
}

/*
 * Metodo que actualiza el estado del robot e interpreta los comandos de control
 * return: si se pudo realizar la actualizacion
 */
bool Controller::updateController()
{
	// Impresion de los mensajes en pantalla
	drawTable();
	DrawText("ROBOT1 CONTROL PANEL", 80, 0, 35, RED);
	fillTable();
	
	// Conversion de imput a movimiento del robot
	bool success = moveRobot();

	// Parpadeo de los ojos del robot
	double time = GetTime();
	double period = time - (long)time;
	bool shouldLEDBeOn = (period < 0.1);
	if (isLEDOn != shouldLEDBeOn)
	{
		char redColor = shouldLEDBeOn ? 0xff : 0;
		vector<char> i = { redColor, 0, 0 };
		cliente->publish("robot1/display/leftEye/set", i);
		cliente->publish("robot1/display/rightEye/set", i);
		isLEDOn = shouldLEDBeOn;
	}

	return success;
}

/*
 * Metodo que permite mover el robot
 * return: si se pudo mover el robot
 */
bool Controller::moveRobot()
{
	//Incluye para gamepad pero no lo pudimos probar pues no tenemos gamepad. Deberia funcionar
	int rotate = (IsKeyDown(KEY_A) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1))
		- (IsKeyDown(KEY_D) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1));

	int moveSideways = (IsKeyDown(KEY_RIGHT) ||
		IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
		- (IsKeyDown(KEY_LEFT) ||
			IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT));

	int moveForward = (IsKeyDown(KEY_UP) ||
		IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
		- (IsKeyDown(KEY_DOWN) ||
			IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN));

	// Las sumas pueden ser distintos de 1, 0 o -1. Para no aumentar la corriente en los motores
	int multiplicador1 = ((rotate + moveForward - moveSideways) == 0) ? 0 :
		((rotate + moveForward - moveSideways) > 0) ? 1 : -1;
	int multiplicador2 = ((rotate - moveForward - moveSideways) == 0) ? 0 :
		((rotate - moveForward - moveSideways) > 0) ? 1 : -1;
	int multiplicador3 = ((rotate - moveForward + moveSideways) == 0) ? 0 :
		((rotate - moveForward + moveSideways) > 0) ? 1 : -1;
	int multiplicador4 = ((rotate + moveForward + moveSideways) == 0) ? 0 :
		((rotate + moveForward + moveSideways) > 0) ? 1 : -1;

	// si se trata de una rotacion, utilizo un factor de escala menor
	float isrotation = (rotate == 0) ? 1.0F : 0.01F;

	bool success[4];
	success[0] = actualizarMotor(1, MOVE_CURRENT * multiplicador1 * isrotation);
	success[1] = actualizarMotor(2, MOVE_CURRENT * multiplicador2 * isrotation);
	success[2] = actualizarMotor(3, MOVE_CURRENT * multiplicador3 * isrotation);
	success[3] = actualizarMotor(4, MOVE_CURRENT * multiplicador4 * isrotation);

	for (int i = 0; i < 4; i++)
	{
		if (!success[i])
		{
			return false;
		}
	}
	return true;	
}

/*
 * Metodo que actualiza la corriente en los motores del robot
 * param n: el numero del motor a actualizar
 * param current: la corriente que se le quiere dar al motor
 * return: si se pudo realizar la actualizacion
 */
bool Controller::actualizarMotor(int n, float current)
{
	vector<char> i = getVectorFromFloat(current);
	bool success = cliente->publish("robot1/motor" + to_string(n) + "/current/set", i);

	return success;
}

/*
 * Metodo que permite dibujar la tabla
 */
void Controller::drawTable()
{
	DrawLine(40, 100, 540, 100, WHITE);
	DrawLine(40, 160, 540, 160, WHITE);
	DrawLine(40, 220, 540, 220, WHITE);
	DrawLine(40, 280, 540, 280, WHITE);
	DrawLine(95, 40, 95, 340, WHITE);
	DrawLine(245, 40, 245, 340, WHITE);
	DrawLine(395, 40, 395, 340, WHITE);

	DrawText("Nr", 40, 65, 25, WHITE);
	DrawText("1", 55, 125, 25, WHITE);
	DrawText("2", 55, 185, 25, WHITE);
	DrawText("3", 55, 245, 25, WHITE);
	DrawText("4", 55, 305, 25, WHITE);
	DrawText("Voltage", 125, 65, 25, WHITE);
	DrawText("Current", 260, 65, 25, WHITE);
	DrawText("Temperature", 400, 65, 25, WHITE);

	DrawText("Use arrows to move, [A] or [D] to turn", 50, 375, 14, WHITE);
}

/*
 * Metodo que permite llenar la tabla
 */
void Controller::fillTable()
{
	vector<MQTTMessage> mensajes = cliente->getMessages();

	for (auto& msj : mensajes) 
	{
		float floatpayload = getFloatFromVector(msj.payload);

		//find devuelve -1 si no se encontro el texto. Para que no entre al if le sumamos 1.
		if (1 + msj.topic.find("1/voltage", 0))
		{
			drawPayload(floatpayload, 125, 125, 24.0F);
		}
		else if (1 + msj.topic.find("2/voltage", 0))
		{
			drawPayload(floatpayload, 125, 185, 24.0F);
		}
		else if (1 + msj.topic.find("3/voltage", 0))
		{
			drawPayload(floatpayload, 125, 245, 24.0F);
		}
		else if (1 + msj.topic.find("4/voltage", 0))
		{
			drawPayload(floatpayload, 125, 305, 24.0F);
		}
		else if (1 + msj.topic.find("1/current", 0))
		{
			drawPayload(floatpayload, 270, 125, 10.0F);
		}
		else if (1 + msj.topic.find("2/current", 0))
		{
			drawPayload(floatpayload, 270, 185, 10.0F);
		}
		else if (1 + msj.topic.find("3/current", 0))
		{
			drawPayload(floatpayload, 270, 245, 10.0F);
		}
		else if (1 + msj.topic.find("4/current", 0))
		{
			drawPayload(floatpayload, 270, 305, 10.0F);
		}
		//para las temperaturas, la funcion evaluara como warning que la temperatura sea menor
		//a -35 grados. Esto no tiene sentido, pues no habra temperaturas negativas
		//Sin embargo, no vale la pena crear otra funcion solo para la temperatura
		else if (1 + msj.topic.find("1/temperature", 0))
		{
			drawPayload(floatpayload, 420, 125, 35.0F);
		}
		else if (1 + msj.topic.find("2/temperature", 0))
		{
			drawPayload(floatpayload, 420, 185, 35.0F);
		}
		else if (1 + msj.topic.find("3/temperature", 0))
		{
			drawPayload(floatpayload, 420, 245, 35.0F);
		}
		else if (1 + msj.topic.find("4/temperature", 0))
		{
			drawPayload(floatpayload, 420, 305, 35.0F);
		}
	}
}

/*
 * Metodo que permite escribir el payload en la tabla
 * float payload: el valor a escribir en la tabla
 * int posx: la posicion en el eje x para el texto
 * int posy: la posicion en el eje y para el texto
 * float condition: el maximo valor que puede tomar el payload
 */
void Controller::drawPayload(float payload, int posx, int posy, float condition)
{
	string stringpayload = to_string(payload);
	char* msjpayload = (char*)(stringpayload.c_str());

	if(payload >= condition || payload <= -condition)
	{
		DrawText("WARNING", posx, posy, 20, WHITE);
	}
	else
	{
		DrawText(msjpayload, posx, posy, 20, WHITE);
	}
}