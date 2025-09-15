#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


const int WIDTH = 850;
const int HEIGHT = 450;

std::vector<Vector2> points;

void UpdateDrawFrame();

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "Connect Dots");
	points.push_back({0, 0});

	#if defined(PLATFORM_WEB)
    	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
	#else
		SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
		//--------------------------------------------------------------------------------------

		// Main game loop
		while (!WindowShouldClose())    // Detect window close button or ESC key
		{
			UpdateDrawFrame();
		}
	#endif

	CloseWindow();

	return 0;
}

bool CellIsCCW(std::vector<Vector2> points)
{
    if (points.size() < 3)
        return true;

    float total = 0;
    for (size_t i = 0; i < points.size() - 1; i++)
        total += (points[i + 1].x - points[i].x) * (points[i + 1].y + points[i].y);

    return total < 0;
}

bool mode = false;
bool first = true;

void UpdateDrawFrame()
{
	BeginDrawing();
	
	ClearBackground(BLACK);

	if (!mode)
		DrawTriangleFan(&points[0], points.size(), WHITE);
	else
		DrawLineStrip(&points[0], points.size(), WHITE);

	std::cout << CellIsCCW(points) << '\n';
	
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		if (first)
		{
			points[0] = GetMousePosition();
			first = false;
		}
		else
		{
			// Vector2 temp = points[0];
			// points[0] = points[points.size()-1];
			// points[points.size()-1] = temp;
			
			points.push_back(GetMousePosition());
		}
	}

	for (Vector2& point : points)
		DrawCircleV(point, 3, WHITE);

	std::string mode_text = "MODE " + (int)mode;
	if (GuiButton(Rectangle{0, 0, 100, 50}, "MODE"))
		mode = !mode;

	EndDrawing();
}