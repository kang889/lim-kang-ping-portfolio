#include "tree.hpp"

Tree::Tree(Vector2 pos, Texture2D tex)
{
    position = pos;
    image = tex;

}

Tree::~Tree()
{
    
    
}

void Tree::Draw() const
{
    DrawTextureV(image, position, WHITE);
}

Vector2 Tree::Getposition() const
{
    return position;
}

Rectangle Tree::Getrect() const
{
    return {position.x, position.y, float(image.width), float(image.height)};
}