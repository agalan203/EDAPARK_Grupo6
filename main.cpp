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

	const int screenWidth = 350;
	const int screenHeight = 200;
	
	InitWindow (screenWidth, screenHeight, "EDA PARK");
	
    Controller controlador;

	SetTargetFPS(10);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		controlador.updateController();
		EndDrawing();
	}
	CloseWindow();
	
    return 0;
}