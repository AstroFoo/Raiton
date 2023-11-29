#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H

#include "SDL.h"

#include "entity.h"
#include "hitbox.h"

class EnemyBullet : public Entity
{
    public:
        EnemyBullet();
        EnemyBullet(SDL_Texture &texture, std::size_t screen_width, std::size_t screen_height);
        EnemyBullet(float x, float y, const EnemyBullet &prefab, float angle, float speed);
        ~EnemyBullet();
        EnemyBullet(const EnemyBullet &source);
        EnemyBullet &operator=(const EnemyBullet &source);
        EnemyBullet(EnemyBullet &&source);
        EnemyBullet &operator=(EnemyBullet &&source);

        Hitbox GetHitbox();
        bool IsOutOfBounds();
        void Update(Uint32 time_elapsed);

    private:
        float _angle = 0.0f;
        float _speed;

        std::size_t _screen_width;
        std::size_t _screen_height;
};

#endif