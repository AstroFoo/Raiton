#include "playerBullet.h"
#include <cmath>
#include <iostream>
#include <memory>
#include "SDL.h"

PlayerBullet::PlayerBullet(){}

PlayerBullet::PlayerBullet(SDL_Texture &texture_)
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

PlayerBullet::PlayerBullet(float x_, float y_, const PlayerBullet &prefab) : PlayerBullet(prefab)
{
    x = x_;
    y = y_;
}

PlayerBullet::~PlayerBullet(){}

PlayerBullet::PlayerBullet(const PlayerBullet &source)
{
    x = source.x;
    y = source.y;
    texture = source.texture;
    width = source.width;
    height = source.height;
}

PlayerBullet &PlayerBullet::operator=(const PlayerBullet &source)
{
    if (this == &source)
        return *this;

    x = source.x;
    y = source.y;
    texture = source.texture;
    width = source.width;
    height = source.height;

    return *this;
}

PlayerBullet::PlayerBullet(PlayerBullet &&source)
{
    x = source.x;
    y = source.y;
    texture = source.texture;
    width = source.width;
    height = source.height;

    source.texture = nullptr;
}

PlayerBullet &PlayerBullet::operator=(PlayerBullet &&source)
{
    if (this == &source)
        return *this;

    x = source.x;
    y = source.y;
    texture = source.texture;
    width = source.width;
    height = source.height;

    source.texture = nullptr;

    return *this;
};

bool PlayerBullet::IsOutOfBounds()
{
    return y + height < 0;
}

int PlayerBullet::GetDamage()
{
    return _damage;
}

Hitbox PlayerBullet::GetHitbox()
{
    Hitbox hitbox;
    hitbox.x = x;
    hitbox.y = y;
    hitbox.width = width;
    hitbox.height = height;
    return hitbox;
}

void PlayerBullet::Update(Uint32 time_elapsed)
{
    y -= _speed;
}