#pragma once
#include <vector>
#include <raylib.h>
#include "player.hpp"


class BossSlime {
    public:
     BossSlime(Vector2 pos, Player& player);
     ~BossSlime();
     void Draw();
     void Update();
     void ApplyKnockBack(Vector2 direction, float force);
     Rectangle Getrect();
     Vector2 Getposition();
     int live;
     bool dead = false;
     bool attack = false;
     bool attackTimer;
     Texture2D idletexture;


    private:
     Texture2D runtexture;
     Texture2D attacktexture;
     Texture2D deadtexture;
     Vector2 position;
     Player& player;
     float speed;
     bool moving = false;
     int idleframe;  // To keep track of the idle animation frame
     int deadframe;   // To keep track of the run animation frame
     float frametimer; // Timer to control animation frame rate
     const float framespeed = 0.1f; 
     bool isKnockedBack = false;
     float knockBackTime = 0.5f;
};