/*
 * EDAPark
 *
 * 22.08 EDA
 * TP 2
 * Santiago Michelotti y Albertina Galan
 *
 * Main module
 */
 
#include "Controller.h"
#include "raylib.h"


int main(){

	const int screenWidth = 500;
	const int screenHeight = 270;
	
	InitWindow(screenWidth, screenHeight, "EDA PARK");
	
    Controller controlador;
	while (true)
	{
		BeginDrawing();
		ClearBackground(BLACK);
		controlador.updateController();
		EndDrawing();
	}
	CloseWindow();
    return 0;
}