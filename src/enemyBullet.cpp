#include "enemyBullet.h"
#include <cmath>
#include <iostream>
#include <memory>
#include "SDL.h"

EnemyBullet::EnemyBullet(){}

// Constructor used for generating prefabs
EnemyBullet::EnemyBullet(SDL_Texture &texture_, std::size_t screen_width, std::size_t screen_height) :
                         _screen_width(screen_width), _screen_height(screen_height)
{
    x = 0;
    y = 0;
    texture = &texture_;

    // Derived parameters
    int width_, height_;
    SDL_QueryTexture(&texture_, NULL, NULL, &width_, &height_);
    width = width_;
    height = height_;
}

// Constructor for generating new entities from a prefab
EnemyBullet::EnemyBullet(float x_, float y_, const EnemyBullet &prefab, float angle, float speed) : EnemyBullet(prefab)
{
    x = x_;
    y = y_;
    _angle = angle;
    _speed = speed;
}

EnemyBullet::~EnemyBullet(){}

EnemyBullet::EnemyBullet(const EnemyBullet &source)
{
    x = source.x;
    y = source.y;
    width = source.width;
    height = source.height;
    texture = source.texture;

    _screen_width = source._screen_width;
    _screen_height = source._screen_height;
    _angle = source._angle;
}

EnemyBullet &EnemyBullet::operator=(const EnemyBullet &source)
{
    if (this == &source)
        return *this;

    x = source.x;
    y = source.y;
    width = source.width;
    height = source.height;
    texture = source.texture;
    _screen_width = source._screen_width;
    _screen_height = source._screen_height;
    _angle = source._angle;

    return *this;
}

EnemyBullet::EnemyBullet(EnemyBullet &&source)
{
    x = source.x;
    y = source.y;
    width = source.width;
    height = source.height;
    texture = source.texture;
    _screen_width = source._screen_width;
    _screen_height = source._screen_height;
    _angle = source._angle;

    source.texture = nullptr;
}

EnemyBullet &EnemyBullet::operator=(EnemyBullet &&source)
{
    if (this == &source)
        return *this;

    x = source.x;
    y = source.y;
    width = source.width;
    height = source.height;
    texture = source.texture;
    _screen_width = source._screen_width;
    _screen_height = source._screen_height;
    _angle = source._angle;

    source.texture = nullptr;

    return *this;
};

bool EnemyBullet::IsOutOfBounds()
{
    return (y > _screen_height) || (y + height < 0) || (x > _screen_width) || (x - width < 0);
}

Hitbox EnemyBullet::GetHitbox()
{
    Hitbox hitbox;
    hitbox.x = x;
    hitbox.y = y;
    hitbox.width = width;
    hitbox.height = height;
    return hitbox;
}

void EnemyBullet::Update(Uint32 time_elapsed)
{
    x += _speed*sin(_angle);
    y += _speed*cos(_angle);
}