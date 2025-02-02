#include "game.hpp"
#include "raymath.h"
#include "raylib.h"
#include <unistd.h>

Game::Game()
{
    camera.target = player.Getposition();  // Follow the player's position
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }; // Center camera
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    GameSound();
    
    Generatetrees();
    GenerateSlime();
    bordertexture = LoadTexture("Graphics/tree.png");
    slimeKilled = 0;
    
    
    
   
}

Game::~Game()
{
    UnloadTexture(treetexture);
    UnloadTexture(bordertexture);
    UnloadTexture(idletexture);
    UnloadTexture(runtexture);
    UnloadSound(grass);
    UnloadMusicStream(music);
}

void Game::Update(){
    player.Update();

    UpdateMusicStream(music);

    Handleinput();

    for (Slime& slime : slimes){
        if(slime.isalive){
        slime.Update();
        }
    }

    

    Checkforpunch();


    if (showMessage){
        messageTimer -= GetFrameTime();
        if (messageTimer <= 0.0f){
            showMessage = false;
        }
    }

    if (slimeKilled >= 15 && !hasGeneratedBossslime){
        GenerateBossSlime();
        hasGeneratedBossslime = true;
        GenerateText("The King is Here! Kill it!", 5.0f);
    }
    
    if(hasGeneratedBossslime && bossSlime){
    bossSlime->Update();
    }

    

    
}

void Game::Draw()
{

    camera.target = player.Getposition();
    BeginMode2D(camera);

    
    player.Draw();

    //for (int x = -5000; x <= 5000; x += 50)
        //DrawLine(x, -5000, x, 5000, LIGHTGRAY);
    //for (int y = -5000; y <= 5000; y += 50)
        //DrawLine(-5000, y, 5000, y, LIGHTGRAY);

    for (const Tree& tree : trees) {
        tree.Draw();  // Draw each tree in the world
    }

    for (Slime& slime : slimes){
        slime.Draw();
    }

    if(bossSlime && hasGeneratedBossslime){
        bossSlime->Draw();
    }

    if (slimeKilled == 0 &&player.isalive){
       DrawText("Arrow key to move, SPACE BAR to attack", player.position.x - (player.image.width / 6), player.position.y + player.image.height, 20, YELLOW);
       DrawText("RED BAR above you is your HEALTH!", player.position.x- (player.image.width / 8), player.position.y + (player.image.height * 1.2), 20, YELLOW);
    }

    if (!player.isalive){
     DrawText("GAMEOVER!, PRESS R TO RESPAWN!", player.position.x - (player.image.width / 6), player.position.y + player.image.height, 20, YELLOW);
    }
        

    if(showMessage){
        DrawText(currentMessage.c_str(), player.position.x -(player.image.width / 6), player.position.y + player.image.height, 20, YELLOW);
    }


    DrawBordertiles();



    EndMode2D(); // End camera mode
}

void Game::Handleinput()
{
    Vector2 oldposition = player.Getposition();

   if(player.isalive){
    if(IsKeyDown(KEY_LEFT)) {
        player.Moveleft();
        player.moving = true;

    } else if (IsKeyDown(KEY_RIGHT)){
        player.Moveright();
        player.moving = true;
    } else if (IsKeyDown(KEY_UP)) {
        player.Moveup();
        player.moving = true;
    } else if (IsKeyDown(KEY_DOWN)){
        player.Movedown();
        player.moving = true;
    } else {
        player.moving = false;
    }

    if(player.moving && !IsSoundPlaying(grass)){
        PlaySound(grass);
    }
    
   
    if(Checkfortreecollision()){
        player.Setposition(oldposition);
    }
   }

    if (IsKeyPressed(KEY_R)){
        GameReset();
    }
}

void Game::Generatetrees()
{
    treetexture = LoadTexture("Graphics/tree.png");

    if (treetexture.id == 0) { // Check if the texture was loaded correctly
        printf("Failed to load tree texture!\n");
        return;
     } else {
        printf("Tree texture loaded successfully!\n");
     }

    int maxtrees = 15;
    float mindistance = 150.0f;
    float minplayerdistance = 150.0f;
    Vector2 playerspawnpoint = player.Getposition();


    while (trees.size() < maxtrees){
        Vector2 position = {
            static_cast<float>(GetRandomValue(-900, 900)), 
            static_cast<float>(GetRandomValue(-900, 900))
        };
        bool validposition = true;
        for (const Tree& tree : trees){
            if(Vector2Distance(position, tree.Getposition()) < mindistance){
                validposition = false;
                break;
            }
        }
        if (validposition && Vector2Distance(position, playerspawnpoint) < minplayerdistance){
            validposition = false;
        }

        if (validposition){
        trees.emplace_back(position, treetexture);
        }
    }


}

void Game::DrawBordertiles()
{ 

    int tilewidth = bordertexture.width;
    int tileheight = bordertexture.height;


    //Draw top border
    for (int row = 0; row < 8; row++){
        int startY = -1000 - tileheight;
        int ypos = startY + row * (-tileheight);
        for (int x = (-1000 - (tilewidth * 10)); x < 1000 + (tilewidth * 8); x += tilewidth) {
          DrawTexture(bordertexture, x, ypos, WHITE); 
     }

    }

    // Draw bottom border
    for (int row = 0; row < 8; row++){
        int startY = 1000 + tileheight;
        int ypos = startY + row * tileheight;
        for (int x = -1000 - (tilewidth * 8); x <1000 + (tilewidth * 8); x += tilewidth){
            DrawTexture(bordertexture, x, ypos, WHITE);
        }
    }

    int numcolumn = 8;

    // Draw left border
    for (int column = 0; column < 10; column++){
        int startX = -1000 - tilewidth;
        int xpos = startX + column * (-tilewidth);
        for (int y = -1000 - tileheight; y < 1000 + tileheight; y+= tileheight){
            DrawTexture(bordertexture, xpos, y, WHITE);
        }
    }

    // Draw right border
    for (int column = 0; column < numcolumn; column++){
        int startX = 1000;
        int xpos = startX + column * tilewidth;
        for (int y = -1000; y< 1000 + tileheight; y+= tileheight){
            DrawTexture(bordertexture, xpos, y, WHITE);

        }

    }
    // DrawRectangleRec(player.Getrect(), RED);
    // for (Slime& slime : slimes){
    //    DrawRectangleRec(slime.Getrect(), BLUE);
   // }
}

void Game::GenerateSlime()
{
    idletexture = LoadTexture("Graphics/Slime_idle_spritesheet.png");
    runtexture = LoadTexture("Graphics/slime_run_spritesheet.png");

     for (int i = 0; i < 5; ++i) {  // Add 5 slimes, for example
        Vector2 slimePos = { static_cast<float>(GetRandomValue(-1000, 1000)), static_cast<float>(GetRandomValue(-1000, 1000)) };
        Slime newSlime(slimePos, idletexture, runtexture, player);
        slimes.push_back(newSlime);
     }
     
}

void Game::GenerateSlime2()
{
    idletexture = LoadTexture("Graphics/Slime_idle_spritesheet.png");
    runtexture = LoadTexture("Graphics/slime_run_spritesheet.png");
        for (int i = 0; i < 20; ++i){
            Vector2 slimepos = { static_cast<float>(GetRandomValue(-1000, 1000)), static_cast<float>(GetRandomValue(-1000, 1000)) };
            Slime newSlime(slimepos, idletexture, runtexture, player);
            slimes.push_back(newSlime);
     }
}

void Game::GenerateBossSlime()
{

    Vector2 bossPos = { static_cast<float>(GetRandomValue(-1000, 1000)), static_cast<float>(GetRandomValue(-1000, 1000)) };
    bossSlime = std::make_unique<BossSlime>(bossPos, player);  // Assuming bossSlime is a unique_ptr

    for (int i = 0; i < 25; ++i){
            idletexture = LoadTexture("Graphics/Slime_idle_spritesheet.png");
            runtexture = LoadTexture("Graphics/slime_run_spritesheet.png");
            Vector2 slimepos = { static_cast<float>(GetRandomValue(-1000, 1000)), static_cast<float>(GetRandomValue(-1000, 1000)) };
            Slime newSlime(slimepos, idletexture, runtexture, player);
            slimes.push_back(newSlime);
        }


}

bool Game::Checkfortreecollision()
{
    for (const Tree& tree : trees){
        if(CheckCollisionRecs(player.Getrect(), tree.Getrect()))
        {
            return true;
        }
    } return false;

}

void Game::GenerateText(const std::string& text, float duration)
{
  currentMessage = text;
  messageTimer = duration;
  showMessage = true;
}

void Game::GameSound()
{
    grass = LoadSound("Sounds/grass.wav");
    attack = LoadSound("Sounds/attack.wav");
    music = LoadMusicStream("Sounds/battle.wav");
    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);

}

void Game::GameReset()
{
    if (!player.isalive){
        slimes.clear();
        slimeKilled = 0;
        hasGeneratedBossslime = false;
        player.live = 1000;
        player.isalive = true;
        GenerateSlime();
        

    }
}

void Game::Checkforpunch(){

    for (auto& slime : slimes){
       if(slime.isalive){
        if(CheckCollisionRecs(player.GetAttackRect(), slime.Getrect()) &&player.isAttacking){
        Vector2 knockBackDirection = Vector2Normalize(Vector2Subtract(slime.Getposition(), player.Getposition()));
        slime.ApplyKnockBack(knockBackDirection, 80.0f);
        slime.live -= 1;
        if(slime.live <= 0){
            slime.isalive = false;
            slimeKilled += 1;
            printf("Slime killed! Current slimeKilled: %d\n", slimeKilled);
        }
      }
     }
    }

    if(bossSlime && !bossSlime->dead){
     if(CheckCollisionRecs(player.GetAttackRect(), bossSlime->Getrect()) &&player.isAttacking){
        Vector2 KnockBackDirection = Vector2Normalize(Vector2Subtract(bossSlime->Getposition(), player.Getposition()));
        bossSlime->ApplyKnockBack(KnockBackDirection, 80.0f);
        bossSlime->live -= 1;
        if(bossSlime->live <=0){
            bossSlime->dead = true;
            GenerateText("You are now the King of this jungle!", 2.0f);
        }
     }
    }

    if (slimeKilled >= 3 && !hasgeneratedslime) {
        GenerateSlime2();
        hasgeneratedslime = true; // Make sure this only happens once
        GenerateText("More Slime coming!, Hunt them all!", 5.0f);
    }


    for (Slime& slime : slimes){
      if(slime.isalive){
        if(CheckCollisionRecs(player.Getrect(), slime.Getrect())){
            player.live -= 0.1;
            player.isHit = true;
            player.hitTimer = 0.2f;
        }
            if (player.live <= 0){
            player.isalive = false;
      }
        
     }
    }
    if(bossSlime && !bossSlime->dead){
   
     if (CheckCollisionRecs(player.Getrect(), bossSlime->Getrect())){
        player.live -= 1;
        if (player.live <= 0){
            player.isalive = false;

        }
        player.isHit = true;
        player.hitTimer = 0.5f;
        bossSlime->attack = true;
    }
    }


 
}
