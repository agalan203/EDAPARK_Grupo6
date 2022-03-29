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

int main(void)
{
    Controller controlador;
    for (int i = 1; i < 5; i++)
    {
        if (controlador.cliente->subscribe("robot1/motor" + to_string(i) + "/voltage"))
        {
            cout << "Subscription to volage" + to_string(i) + " succesful" << endl;
        }
        else
        {
            cout << "Subscription to volage" + to_string(i) + " not succesful" << endl;
        }
        if (controlador.cliente->subscribe("robot1/motor" + to_string(i) + "/current"))
        {
            cout << "Subscription to current" + to_string(i) + " succesful" << endl;
        }
        else
        {
            cout << "Subscription to current" + to_string(i) + " not succesful" << endl;
        }
    }
    
    
    vector<char> data(sizeof(float));
    float payload = 2.0F;
    memcpy(data.data(), &payload, sizeof(float));

    if (controlador.cliente->publish("robot1/motor1/current/set", data))
    {
        cout << "Publish succesful" << endl;
    }
    
    controlador.cliente->disconnect();
    
     if (!controlador.cliente->isConnected())
     {
         cout << "Disconnection Succesful" << endl;
     }

    return 0;
}
