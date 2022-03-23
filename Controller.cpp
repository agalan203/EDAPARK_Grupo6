#include "Controller.h"

using namespace std;

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

Controller::~Controller() {
	for (int i = 1; i < 5; i++)
	{
		cliente->unsubscribe("robot1/motor"+to_string(i)+"/voltage");
		cliente->unsubscribe("robot1/motor" + to_string(i) + "/current");
		cliente->unsubscribe("robot1/motor" + to_string(i) + "/temperature");
	}
	cliente->disconnect();
}

float Controller::getFloatFromArray(std::vector<char> payload) {

	//https://stackoverflow.com/questions/6417438/c-convert-vectorchar-to-double

	float convert = 0.0;
	memcpy(&convert, &payload, sizeof(float));
	return convert;

}

std::vector<char> Controller::getArrayFromFloat(float payload) {

	//https://stackoverflow.com/questions/52741039/how-to-convert-float-to-vectorunsigned-char-in-c

	vector<char> data(sizeof(float));
	memcpy(data.data(), &payload, sizeof(float));
	return data;
}

void Controller::updateController() {

	vector<MQTTMessage> mensajes = cliente->getMessages();
	
	/*for (int i = 0; i < mensajes.size(); i++)
	{
		// Imprimir en una tabla todos los valores //cout << mensajes[i].topic << endl; //vector char a float
		cout << mensajes[i].topic << " " << getFloatFromArray(mensajes[i].payload) << endl;

	}
	cout << "sali del loop" << endl << endl << endl << endl;
	
	switch (GetKeyPressed())
	{
	case KEY_UP:
		moveForward();
		break;
	case KEY_DOWN:
		moveBackward();
	case KEY_RIGHT:
		turnRight();
	case KEY_LEFT:
		turnLeft();
	default:
		break;
	}
	*/
	if (IsKeyDown(KEY_UP))
	{
		if (IsKeyDown(KEY_RIGHT))
		{
			moveDiagonal(DIAGONAL_CUAD1);
		}
		else if (IsKeyDown(KEY_LEFT))
		{
			moveDiagonal(DIAGONAL_CUAD2);
		}
		else
		{
			cout << "lei la tecla up" << endl << endl << endl;
			moveForward();
		}
		
	}
	else if (IsKeyDown(KEY_DOWN))
	{
		if (IsKeyDown(KEY_RIGHT))
		{
			moveDiagonal(DIAGONAL_CUAD4);
		}
		else if (IsKeyDown(KEY_LEFT))
		{
			moveDiagonal(DIAGONAL_CUAD3);
		}
		else
		{
			moveBackward();
		}

	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		turnRight();
	}
	else if (IsKeyDown(KEY_LEFT))
	{
		turnLeft();
	}
}

void Controller::moveForward() {
	motoresHorario(1,3);
	motoresAntiHorario(2, 4);
}

void Controller::moveBackward() {
	motoresHorario(2, 4);
	motoresAntiHorario(1, 3);
}

void Controller::turnRight() {
	motoresHorario(1, 2, 3, 4);
}

void Controller::turnLeft(){
	motoresAntiHorario(1, 2, 3, 4);
}

void Controller::moveDiagonal(int opcion) {
	switch (opcion)
	{
	case DIAGONAL_CUAD1:
		motoresAntiHorario(2, 4);
		motoresDetenidos(1, 3);
		break;
	case DIAGONAL_CUAD2:
		motoresAntiHorario(1, 3);
		motoresDetenidos(2, 4);
		break;
	case DIAGONAL_CUAD3:
		motoresHorario(2, 4);
		motoresDetenidos(1, 3);
		break;
	case DIAGONAL_CUAD4:
		motoresHorario(1, 3);
		motoresDetenidos(2, 4);
		break;
	default:
		break;
	}
}

void Controller::motoresHorario(int n1, int n2) {

	vector<char> i = getArrayFromFloat(5.0F);
	//publish Amper + para n1 y n2 EL ERROR ESTA EN PUBLISH
	cliente->publish("robot1/motor" + to_string(n1) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n2) + "/current/set",i);
}
void Controller::motoresAntiHorario(int n1, int n2) {
	vector<char> i = getArrayFromFloat(-5.0F);
	//publish Amper - para n1 y n2
	cliente->publish("robot1/motor" + to_string(n1) + "/current/set",i);
	cliente->publish("robot1/motor" + to_string(n2) + "/current/set", i);
}

void Controller::motoresHorario(int n1, int n2, int n3, int n4) {

	vector<char> i = getArrayFromFloat(5.0F);
	//publish Amper + para n1, n2, n3, n4
	cliente->publish("robot1/motor1/current/set", i);
	cliente->publish("robot1/motor2/current/set", i);
	cliente->publish("robot1/motor" + to_string(n3) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n4) + "/current/set", i);
}
void Controller::motoresAntiHorario(int n1, int n2, int n3, int n4) {
	
	vector<char> i = getArrayFromFloat(-5.0F);
	//publish Amper - para n1, n2, n3, n4
	cliente->publish("robot1/motor" + to_string(n1) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n2) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n3) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n4) + "/current/set", i);
}

void Controller::motoresDetenidos(int n1, int n2) {
	vector<char> i = getArrayFromFloat(0.0F);
	cliente->publish("robot1/motor" + to_string(n1) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n2) + "/current/set", i);
}

void Controller::motoresDetenidos(int n1, int n2, int n3, int n4) {
	
	vector<char> i = getArrayFromFloat(0.0F);
	cliente->publish("robot1/motor" + to_string(n1) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n2) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n3) + "/current/set", i);
	cliente->publish("robot1/motor" + to_string(n4) + "/current/set", i);
}