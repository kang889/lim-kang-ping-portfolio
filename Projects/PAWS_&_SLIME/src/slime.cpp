#include "slime.hpp"
#include "raymath.h"

Slime::Slime(Vector2 pos, Texture2D idletexture, Texture2D runtexture, Player& player) 
: player(player) 
{

    position = pos;
    this -> idletexture = idletexture;
    this -> runtexture = runtexture;
    speed = 1.5f;
    idleframe = 0;
    runframe = 0;
    frametimer = 0.0f;
    live = 2;
}

Slime::~Slime() 
{


}

void Slime::Draw(){

  //DrawRectangleRec(Getrect(), GREEN);
  

  if(isalive){
    int scale = 3;
    int framewidth = moving ? runtexture.width / 6 : idletexture.width / 6;
    int frameheight = moving ? runtexture.height : idletexture.height;
    bool faceRight = (player.Getposition().x > (position.x + framewidth)) || (player.Getposition().x == (position.x + framewidth));

    Rectangle sourcerec = {float((moving ? runframe: idleframe) * framewidth), 0.0f, float(framewidth), float(frameheight)};
    if (!faceRight) {
        sourcerec.width = -float(framewidth);  // Adjust the x position for flipping
     } 
    Rectangle destrec = {position.x, position.y, float(framewidth) * scale, float(frameheight) * scale};
    Texture2D texture = moving ? runtexture : idletexture;
    DrawTexturePro(texture, sourcerec, destrec, {0,0}, 0.0f, WHITE);
  }

 }

void Slime::Update(){

    Vector2 playerpos = player.Getposition();
    if(player.isalive){
     if (Vector2Distance(position, playerpos) < 800.0f){
        moving = true;
        Vector2 direction = Vector2Normalize(Vector2Subtract(playerpos, position));
        position.x += direction.x * speed;
        position.y += direction.y * speed;
    } else{
        moving = false;
     }
    }

    frametimer += GetFrameTime();
    if (moving){
        if(frametimer >= framespeed){
            runframe = (runframe + 1) % 6;
            frametimer = 0.0f;
     }
  } else {
        if(frametimer >= framespeed){
            idleframe = (idleframe + 1) % 6;
            frametimer = 0.0f;
        }
     }
    if (isKnockedBack){
        printf("knocked back active");
        if(GetTime() - knockBackTime >= 0.5f) {
            isKnockedBack = false;
        }
    }
}

void Slime::ApplyKnockBack(Vector2 direction, float force)
{
    if(!isKnockedBack){
        position.x += direction.x * force;
        position.y += direction.y * force;
        isKnockedBack = true;
        knockBackTime = 0.5f;


    }

}

__attribute__((aligned(4))) Rectangle Slime::Getrect()
{
    
    return {position.x, position.y, (float(idletexture.width) / 6) * 3.0f, float(idletexture.height) * 3.0f};
}

Vector2 Slime::Getposition()
{
    return position;
}
