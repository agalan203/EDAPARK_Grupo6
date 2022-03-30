/*
 * EDAPark
 *
 * 22.08 EDA
 * TP 2
 * Santiago Michelotti y Albertina Galan
 *
 * Test module
 */

#include "Controller.h"

using namespace std;

void print(string s)
{ 
    cout << endl;
    cout << s << endl;
}

int fail()
{ 
    cout << "FAIL" << endl << endl;
    return 1;
}

int pass()
{ 
    cout << "PASS" << endl << endl;
    return 0;
}

int main()
{
	const int screenWidth = 600;
	const int screenHeight = 410;

	InitWindow(screenWidth, screenHeight, "EDA PARK");

    print("For tests to work EDAPark must be open");

    Controller controlador;

    print("Controller() is able to establish connection...");
    if(!controlador.cliente->isConnected())
        return fail();
    pass();

    print("updateController() is able to update the state of the robot...");
    if(!controlador.updateController())
        return fail();
    pass();

	CloseWindow();

    return 0;
}
