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
	const int screenHeight = 410;

	InitWindow(screenWidth, screenHeight, "EDA PARK");

	Controller controlador;

	SetTargetFPS(10);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		if(!controlador.updateController())
		{
			std::cout << "No se pudo actualizar el estado del robot" << std::endl;
		}
		EndDrawing();
	}
	CloseWindow();

	return 0;
}