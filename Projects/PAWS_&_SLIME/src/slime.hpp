#pragma once
#include <vector>
#include <raylib.h>
#include "player.hpp"

class Slime {
    public:
     Slime(Vector2 pos, Texture2D idletexture, Texture2D runtexture, Player& player);
     ~Slime();
     void Draw();
     void Update();
     void ApplyKnockBack(Vector2 direction, float force);
     __attribute__((aligned(4))) Rectangle Getrect();
     Vector2 Getposition();
     int live;
     bool isalive = true;
     
     

    private:
     Texture2D idletexture;
     Texture2D runtexture;
     Vector2 position;
     float speed;
     bool moving = false;
     Player& player;
     int idleframe;  // To keep track of the idle animation frame
     int runframe;   // To keep track of the run animation frame
     float frametimer; // Timer to control animation frame rate
     const float framespeed = 0.1f; 
     bool isKnockedBack = false;
     float knockBackTime = 0.5f;
};