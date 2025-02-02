#pragma once
#include <raylib.h>
#include <vector>

class Player{

    public:
      Player();
      ~Player();
      void Draw();
      void Moveleft();
      void Moveright();
      void Moveup();
      void Movedown();

    private:
      Texture2D image;
      Vector2 position;
      



};