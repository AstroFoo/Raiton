#include "playerBullet.h"
#include <cmath>
#include <iostream>
#include <memory>
#include "SDL.h"

// Default constructor with default member values
// Note, x, y, width, height and texture belong to the base class "Entity" so they cannot just go into the initializer list.
// Instead, call an Entity constructor for these
PlayerBullet::PlayerBullet() : Entity() {}

PlayerBullet::PlayerBullet(SDL_Texture &texture_) :
                           Entity(0, 0, 0, 0, &texture_)
{
    // Derived parameters
    int width_, height_;
    SDL_QueryTexture(&texture_, NULL, NULL, &width_, &height_);
    width = width_;
    height = height_;
}

PlayerBullet::PlayerBullet(float x_, float y_, const PlayerBullet &prefab) :
                           Entity(x_, y_, prefab.width, prefab.height, prefab.texture) {}

PlayerBullet::~PlayerBullet()
{
    texture = nullptr;
}

PlayerBullet::PlayerBullet(const PlayerBullet &source) :
                           Entity(source.x, source.y, source.width, source.height, source.texture) {}

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

PlayerBullet::PlayerBullet(PlayerBullet &&source) :
                           Entity(source.x, source.y, source.width, source.height, source.texture)
{
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