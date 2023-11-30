#include "entity.h"

float Entity::GetCentreX()
{
    return x + width/2;
}
float Entity::GetCentreY()
{
    return y + height/2;
}

Entity::Entity() : x(0), y(0), width(0), height(0), texture(nullptr) {}

Entity::Entity(float x_, float y_, float width_, float height_, SDL_Texture *texture_) :
               x(x_), y(y_), width(width_), height(height_), texture(texture_) {}