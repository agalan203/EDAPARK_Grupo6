#include "Controller.h"


using namespace std;

Controller::Controller() {

	cliente = new MQTTClient("controller");
	
	if (!(cliente->connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez"))) 
	{
		cout << "No se logró establecer conexión" << endl;
	}
	for (int i = 1; i < 5; i++)
	{
		cliente->subscribe("robot1/motor"+to_string(i)+"/voltage");
		cliente->subscribe("robot1/motor" + to_string(i) + "/current");
		cliente->subscribe("robot1/motor" + to_string(i) + "/temperature");
	}
	
	
}

void Controller::updateController() {

	vector<MQTTMessage> mensajes = cliente->getMessages();

	for (int i = 0; i < mensajes.size(); i++)
	{
		// Imprimir en una tabla todos los valores //cout << mensajes[i].topic << endl; //vector char a float
	}

	if (IsKeyDown(KEY_UP))
	{
		if (IsKeyDown(KEY_RIGHT))
		{
			Controller::moveDiagonal(DIAGONAL_CUAD1);
		}
		else if (IsKeyDown(KEY_LEFT))
		{
			Controller::moveDiagonal(DIAGONAL_CUAD2);
		}
		else
		{
			Controller::moveForward();
		}
		
	}
	else if (IsKeyDown(KEY_DOWN))
	{
		if (IsKeyDown(KEY_RIGHT))
		{
			Controller::moveDiagonal(DIAGONAL_CUAD4);
		}
		else if (IsKeyDown(KEY_LEFT))
		{
			Controller::moveDiagonal(DIAGONAL_CUAD3);
		}
		else
		{
			Controller::moveBackward();
		}

	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		Controller::turnRight();
	}
	else if (IsKeyDown(KEY_LEFT))
	{
		Controller::turnLeft();
	}
}

void Controller::moveForward() {
	for (int i = 1; i < 5; i++)
	{
		//cliente->publish("robot1/motor" + to_string(i) + "/current/set", );//float a vector char
	}
}

Controller::~Controller() {
	cliente->disconnect();
}