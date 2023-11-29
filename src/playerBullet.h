#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include "SDL.h"

#include "entity.h"
#include "hitbox.h"

class PlayerBullet : public Entity
{
    public:
        PlayerBullet();
        PlayerBullet(SDL_Texture &texture);
        PlayerBullet(float x, float y, const PlayerBullet &prefab);
        ~PlayerBullet();
        PlayerBullet(const PlayerBullet &source);
        PlayerBullet &operator=(const PlayerBullet &source);
        PlayerBullet(PlayerBullet &&source);
        PlayerBullet &operator=(PlayerBullet &&source);

        int GetDamage();
        Hitbox GetHitbox();
        bool IsOutOfBounds();
        void Update(Uint32 time_elapsed);

    private:
        int _damage = 1;
        float _speed = 10.0f;
};

#endif