#pragma once
#include <raylib.h>
#include <vector>

class Player{

    public:
      Player();
      ~Player();
      void Update();
      void Draw();
      void Moveleft();
      void Moveright();
      void Moveup();
      void Movedown();
      Vector2 Getposition();
      Rectangle Getrect();
      void Setposition(Vector2 pos);
      bool moving = false;
      Rectangle GetAttackRect();
      int live;
      bool isalive = true;
      bool isHit = false;
      float hitTimer = 0.0f;
      Color hitTint = WHITE;
      Texture2D image;



    private:
      Vector2 position;
      friend class Game;
      Sound flesh;
      Texture2D runimage;
      int idleframe;  // To keep track of the idle animation frame
      int runframe;   // To keep track of the run animation frame
      float frametimer; // Timer to control animation frame rate
      const float framespeed = 0.1f; 
      int direction = 1;
      float scale = 0.5f;
      float minX = 1000.0f;   // Left boundary
      float maxX = 1000.0f;   // Right boundary
      float minY = 1000.0f;   // Top boundary
      float maxY = 1000.0f;   // Bottom boundary
      Texture2D attackImage;        // Texture for the attack animation
      Texture2D deadImage;
      int attackFrame = 0;          // Current frame of the attack animation
      float attackFrameTimer = 0.0f; // Timer to control frame updates
      bool isAttacking = false;     // Whether the player is attacking
      const float attackFrameSpeed = 0.1f; // Time per frame for the attack animation
      float attackRange = 15.0f;
      int attackDamage = 10;
      int deadframe;
      




};