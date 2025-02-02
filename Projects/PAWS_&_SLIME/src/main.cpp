#include <raylib.h>
#include "game.hpp"
#include "raylib.h"
#include <string>
#include "raymath.h"

int main()
{
  Color grey = {82, 190, 128, 255};
  int screenwidth = 800;
  int screenheight = 800;

  InitWindow(screenwidth, screenheight, "PAWS & SLIME");
  InitAudioDevice();
  SetTargetFPS(60);
  


Game game;

while (!WindowShouldClose())
{

    BeginDrawing();
    ClearBackground(grey);
    game.Draw();
    game.Update();
    EndDrawing();
    
}

CloseWindow();
CloseAudioDevice();

}
