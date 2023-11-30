#ifndef ENTITY_H 
#define ENTITY_H

#include "SDL.h"

#include "hitbox.h"

class Entity
{
    public:
        float x;
        float y;
        float width;
        float height;
        SDL_Texture *texture = nullptr;

        bool alive{true}; // Used to tag entities for removal from containers

        float GetCentreX();
        float GetCentreY();
        
        virtual Hitbox GetHitbox() = 0;
        virtual bool IsOutOfBounds() = 0;
        virtual void Update(Uint32 time_elapsed) = 0;

    protected:
        Entity();
        Entity(float x, float y, float width, float height, SDL_Texture *texture);
};

#endif