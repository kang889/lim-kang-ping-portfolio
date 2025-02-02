#pragma once
#include <raylib.h>
#include <vector>

class Tree{
    public:
     Tree(Vector2 pos, Texture2D tex);
     ~Tree();
     void Draw() const;
     void Update();
     Vector2 Getposition() const;
     Rectangle Getrect() const;


    private:
     Vector2 position;
     Texture2D image;


};