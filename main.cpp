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

int main()
{

	const int screenWidth = 600;
	const int screenHeight = 400;

	InitWindow(screenWidth, screenHeight, "EDA PARK");

	Controller controlador;

	SetTargetFPS(5);

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