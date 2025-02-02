#include "player.hpp"
#include "raymath.h"

Player::Player()
{
    image = LoadTexture("Graphics/dpg_idle.png");
    runimage = LoadTexture("Graphics/dog_run.png");
    attackImage = LoadTexture("Graphics/dog_punch.png");
    deadImage = LoadTexture("Graphics/dog_dead.png");
    position.x = (GetScreenWidth() - image.width)/2;
    position.y = GetScreenHeight() - image.height - 100;
    flesh = LoadSound("Sounds/flesh.wav");

    idleframe = 0;
    runframe = 0;
    deadframe = 0;
    frametimer = 0.0f;
    live = 1000;


}


Player::~Player()
{
    UnloadTexture(image);
    UnloadTexture(runimage);
    UnloadTexture(attackImage);
    UnloadTexture(deadImage);
    UnloadSound(flesh);
}

void Player::Update()
{


    if(isHit){
        hitTimer -= GetFrameTime();
        if (hitTimer <= 0.0f ) {
            isHit = false;
            hitTint = WHITE;
        } else {
            hitTint = RED;
        }
    }

    frametimer += GetFrameTime();


    if(!isalive){
        if(frametimer >= framespeed){
            deadframe++;
            frametimer = 0.0f;
        } 
        if(deadframe <= 10){
            deadframe = 9;
        }
        return;
    }

    if (isAttacking){
        attackFrameTimer += GetFrameTime();
        if (attackFrameTimer >= attackFrameSpeed){
            attackFrame = (attackFrame + 1) % 11;
            attackFrameTimer = 0.0f;

            if (attackFrame == 0){
                isAttacking = false;
            } 
        }
    } else if (IsKeyPressed(KEY_SPACE) && !isAttacking){
        isAttacking = true;
        PlaySound(flesh);
        attackFrame = 0;
    }
    
    if (!isAttacking){

     if (moving == true){
        if(frametimer >= framespeed){
            runframe = (runframe + 1) % 8;
            frametimer = 0.0f;
      }
     } else {
        if(frametimer >= framespeed){
            idleframe = (idleframe + 1) % 8;
            frametimer = 0.0f;
        }
      }
    }

}

void Player::Draw()
{

    if(!isalive){
        int framewidth = deadImage.width / 10;
        int frameheight = deadImage.height;
        Rectangle sourcerec = {float(deadframe * framewidth), 0.0f, float(framewidth), float(frameheight)};
        Rectangle destrec = {position.x, position.y, float(framewidth) * scale, float(frameheight) * scale};
        DrawTexturePro(deadImage, sourcerec, destrec, {0,0}, 0.0f, WHITE);
        return;
    }

    if (isAttacking){
        int framewidth = attackImage.width / 11;
        int frameheight = attackImage.height;
        Rectangle sourcerec = {float(attackFrame * framewidth), 0.0f, float(framewidth) * direction, float(frameheight)};
        Rectangle destrec = {position.x, position.y, float(framewidth) * scale, float(frameheight) * scale};
        DrawTexturePro(attackImage, sourcerec, destrec, {0,0}, 0.0f, hitTint);
    }
    else if (moving == true){
        int framewidth = runimage.width / 8;
        int frameheight = runimage.height;
        Rectangle sourcerec = {float(runframe * framewidth), 0.0f, float(framewidth) * direction, float(frameheight)};
        Rectangle destrec = {position.x, position.y, float(framewidth) * scale, float(frameheight) * scale};
        DrawTexturePro(runimage, sourcerec, destrec, {0,0}, 0.0f, hitTint);
    } 
    else{
        int framewidth = image.width / 8;
        int frameheight = image.height;
        Rectangle sourcerec = {float(idleframe * framewidth), 0.0f, float(framewidth) * direction, float(frameheight)};
        Rectangle destrec = {position.x, position.y, float(framewidth) * scale, float(frameheight) * scale};
        DrawTexturePro(image, sourcerec, destrec, {0,0}, 0.0f, hitTint);
    }

    float maxlife = 1000.0f;
    float healthBarWidth = 100.0f;
    float healthBarHeight = 10.0f;
    float healthPercent = live / maxlife;

    Vector2 healthBarPos = { position.x - healthBarWidth / 3, position.y - healthBarHeight - 5.0f };
    DrawRectangle(healthBarPos.x, healthBarPos.y, healthBarWidth, healthBarHeight, GRAY);
    DrawRectangle(healthBarPos.x, healthBarPos.y, healthBarWidth * healthPercent, healthBarHeight, RED);


   
    //DrawRectangleRec(Getrect(), BLUE);
    //DrawRectangleRec(GetAttackRect(), YELLOW);
}

void Player::Moveleft()
{
    position.x = Clamp(position.x - 7, -1000, 1000 - ((image.width/8) * scale));
    direction = -1;
}

void Player::Moveright()
{
    position.x = Clamp(position.x + 7, -1000, 1000 - ((image.width/8) * scale));
    direction = 1;
}

void Player::Moveup()
{
    position.y = Clamp(position.y - 7, -1000, 1000 - (image.height * scale));
}

void Player::Movedown()
{
    position.y = Clamp(position.y + 7, -1000, 990) ;
}

Vector2 Player::Getposition()
{
    return position;
}

Rectangle Player::Getrect()
{
    return {(position.x + 15), position.y, float(image.width/15) * scale, float(image.height) * scale};
}

void Player::Setposition(Vector2 pos)
{
    position = pos;
}

Rectangle Player::GetAttackRect()
{
     return Rectangle {
        position.x + (direction == 1 ? (attackRange + 55.0f) : -attackRange),
        position.y + float((image.height) * scale / 3),
        attackRange,
        (float(image.height) * scale) / 4
    };
}
