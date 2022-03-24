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
	
	for (int i = 0; i < mensajes.size(); i++)
	{
		//cout << mensajes[i].topic << endl;
		//char* msj = const_cast<char*>(mensajes[i].topic.c_str());
		//DrawText(msj,0,i*14,14,WHITE);
		//falta la parte del value y funciona raro
	}

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
	else if (IsKeyUp(KEY_RIGHT)&& IsKeyUp(KEY_LEFT)&& IsKeyUp(KEY_UP)&& IsKeyUp(KEY_DOWN))
	{
		actualizarMotor(1, 2, 3, 4, STOP_CURRENT);
	}
}	

void Controller::moveForward() {
	actualizarMotor(1, MOVE_CURRENT);
	actualizarMotor(2, -MOVE_CURRENT);
	actualizarMotor(3, 4, STOP_CURRENT);
}

void Controller::moveBackward() {
	actualizarMotor(3, MOVE_CURRENT);
	actualizarMotor(4, -MOVE_CURRENT);
	actualizarMotor(1, 2, STOP_CURRENT);
}

void Controller::turnRight() {
	actualizarMotor(1, 2, -TURN_CURRENT);
}

void Controller::turnLeft(){
	actualizarMotor(1, 2, TURN_CURRENT);

}

void Controller::moveDiagonal(int opcion) {
	switch (opcion)
	{
	case DIAGONAL_CUAD1:
		actualizarMotor(2, -MOVE_CURRENT);
		actualizarMotor(4, MOVE_CURRENT);
		actualizarMotor(1, 3, STOP_CURRENT);
		break;
	case DIAGONAL_CUAD2:
		actualizarMotor(3, -MOVE_CURRENT);
		actualizarMotor(1, MOVE_CURRENT);
		actualizarMotor(2, 4, STOP_CURRENT);
		break;
	case DIAGONAL_CUAD3:
		actualizarMotor(4, -MOVE_CURRENT);
		actualizarMotor(2, MOVE_CURRENT);
		actualizarMotor(1, 3, STOP_CURRENT);
		break;
	case DIAGONAL_CUAD4:
		actualizarMotor(1, -MOVE_CURRENT);
		actualizarMotor(3, MOVE_CURRENT);
		actualizarMotor(2, 4, STOP_CURRENT);
		break;
	default:
		break;
	}
}

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
