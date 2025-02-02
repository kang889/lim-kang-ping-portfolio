#include "bossslime.hpp"
#include "raymath.h"

BossSlime::BossSlime(Vector2 pos, Player& player) : position(pos), player(player)
{
    idletexture = LoadTexture("Graphics/Pink/SlimeKingPink_Idle.png");
    runtexture = LoadTexture("Graphics/Pink/SlimeKingPink_Hop.png");
    attacktexture = LoadTexture("Graphics/Pink/SlimeKingPink_Attack.png");
    deadtexture = LoadTexture("Graphics/Pink/SlimeKingPink_Death.png");
    idleframe = 0;
    deadframe = 0;
    frametimer = 0.0f;
    speed = 2.0f;
    live = 20;
}

BossSlime::~BossSlime()
{
    UnloadTexture(idletexture);
    UnloadTexture(runtexture);
    UnloadTexture(attacktexture);
    UnloadTexture(deadtexture);
}

void BossSlime::Draw()
{

    int scale = 5;

    if(dead){
        int framewidth = deadtexture.width / 10;
        int frameheight = deadtexture.height;
        //bool faceright = (player.Getposition().x > (position.x + framewidth));
        Rectangle sourcerec ={float(deadframe * framewidth), 0.0f, float(framewidth), float(frameheight)};
        //if(!faceright){
           // sourcerec.width = -float(framewidth);
        //}
        Rectangle destrec = {position.x - framewidth, position.y - (frameheight * 2), float(framewidth) * scale, float(frameheight) * scale};
        DrawTexturePro(deadtexture, sourcerec, destrec, {0,0}, 0.0f, WHITE);
    } else {

    int framewidth = attack ? attacktexture.width / 9 : (moving ? runtexture.width / 8 : idletexture.width / 8);
    int frameheight = attack ? attacktexture.height : (moving ? runtexture.height : idletexture.height);
    bool faceright = (player.Getposition().x > (position.x + framewidth));

    Rectangle sourcerec = {float(idleframe * framewidth), 0.0f, float(framewidth), float(frameheight)};
    if(!faceright) {
        sourcerec.width = -float(framewidth);
    }
    Rectangle destrec = {position.x - framewidth, 
                     position.y - (frameheight * 2), 
                     float(framewidth) * scale, float(frameheight) * scale};

    Texture2D image = attack ? attacktexture : (moving ? runtexture : idletexture);
    //Vector2 origin = {float(framewidth) / 2.0f, float(frameheight) / 2.0f};  // Origin set to center of the frame

    DrawTexturePro(image, sourcerec, destrec, {0,0}, 0.0f, WHITE);
    }


    float maxlife = 20.0f;
    float healthBarWidth = 100.0f;
    float healthBarHeight = 10.0f;
    float healthPercent = live / maxlife;

    Vector2 healthBarPos = { position.x - healthBarWidth / 3, position.y - healthBarHeight - 5.0f };
    DrawRectangle(healthBarPos.x, healthBarPos.y, healthBarWidth, healthBarHeight, GRAY);
    DrawRectangle(healthBarPos.x, healthBarPos.y, healthBarWidth * healthPercent, healthBarHeight, RED);

    //DrawRectangleRec(Getrect(), RED);
}

void BossSlime::Update()
{
    frametimer += GetFrameTime();
    if(dead){
        if (frametimer >= framespeed) {
        deadframe++;  // Assuming the attack animation has 8 frames
        frametimer = 0.0f;
        }
        if (deadframe >= 10){
            deadframe = 9;

      } return;
     
    }
    if (!dead){
    Vector2 playerpos = player.Getposition();
    //playerpos.y = playerpos.y - (player.image.height * 0.3);
    //playerpos.x = playerpos.x - (player.image.width/8);
    if (Vector2Distance(position, playerpos) < 800.0f){
        moving = true;
        Vector2 direction = Vector2Normalize(Vector2Subtract(playerpos, position));
        position.x += direction.x * speed;
        position.y += direction.y * speed;
    } else {
        moving = false;
    }

    if (attack){
        if (frametimer >= framespeed) {
        idleframe = (idleframe + 1) % 9;  // Assuming the attack animation has 8 frames
        frametimer = 0.0f;

        if (idleframe == 0){
            attack = false;
        }
     }
    } else if (moving) {
        if(frametimer >= framespeed){
        idleframe = (idleframe + 1) % 8;
        frametimer = 0.0f; 
       }
    } else {

        if(frametimer >= framespeed){
        idleframe = (idleframe + 1) % 8;
        frametimer = 0.0f;
    }
   }
  }


   if (isKnockedBack){
        printf("knocked back active");
        if(GetTime() - knockBackTime >= 0.5f) {
            isKnockedBack = false;
        }
    }
}

void BossSlime::ApplyKnockBack(Vector2 direction, float force)
{
    if(!isKnockedBack){
        position.x += direction.x * force;
        position.y += direction.y * force;
        isKnockedBack = true;
        knockBackTime = 0.5f;


    }
}

Rectangle BossSlime::Getrect()
{
    return {position.x, position.y, (float(idletexture.width) / 8) * 3.0f, float(idletexture.height) * 1.2f};
   
}

Vector2 BossSlime::Getposition()
{
    return position;
}
