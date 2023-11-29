#include "entity.h"

float Entity::GetCentreX()
{
    return x + width/2;
}
float Entity::GetCentreY()
{
    return y + height/2;
}