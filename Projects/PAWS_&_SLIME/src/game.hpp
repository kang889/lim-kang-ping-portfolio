#pragma once
#include "player.hpp"
#include "tree.hpp"
#include "slime.hpp"
#include "bossslime.hpp"
#include <raylib.h>
#include "raylib.h"


class Game{
    
    public:
     Game();
     ~Game();
     void Update();
     void Draw();
     void Handleinput();
     void Generatetrees();
     void DrawBordertiles();
     void GenerateSlime();
     void Checkforpunch();
     void GenerateSlime2();
     void GenerateBossSlime();
    



    private:
    Player player;
    Camera2D camera;
    std::vector<Tree> trees;
    Texture2D treetexture;
    Texture2D bordertexture;
    Texture2D idletexture;
    Texture2D runtexture;
    std::vector<Slime> slimes;
    bool Checkfortreecollision();
    void GenerateText(const std::string& text, float duration);
    void GameSound();
    void GameReset();
    void GeneratePlant();
    Sound grass;
    Sound attack;
    Music music;
    int slimeKilled;
    std::string currentMessage;
    float messageTimer = 0.0f;
    bool showMessage = false;
    bool hasgeneratedslime = false;
    bool hasGeneratedBossslime = false;
    std::unique_ptr<BossSlime> bossSlime;  // Declare in Game.hpp


};