#include "enemyBullet.h"
#include <cmath>
#include <iostream>
#include <memory>
#include "SDL.h"

// Default constructor with default member values
// Note, x, y, width, height and texture belong to the base class "Entity" so they cannot just go into the initializer list.
// Instead, call an Entity constructor for these
EnemyBullet::EnemyBullet() : Entity(), _screen_width(0), _screen_height(0), _angle(0), _speed(0) {}

// Constructor used for generating prefabs
// Note, x, y, width, height and texture belong to the base class "Entity" so they cannot just go into the initializer list.
// Instead, call an Entity constructor for these.
// Width and height are set based on the supplied texture
EnemyBullet::EnemyBullet(SDL_Texture &texture_, std::size_t screen_width, std::size_t screen_height) :
                         Entity(0, 0, 0, 0, &texture_),
                         _screen_width(screen_width), _screen_height(screen_height), _angle(0), _speed(0)
{
    // Derived parameters
    int width_, height_;
    SDL_QueryTexture(&texture_, NULL, NULL, &width_, &height_);
    width = width_;
    height = height_;
}

// Constructor for generating new entities from a prefab.  It copies data from the prefab ecept for
// x, y, angle and speed, which are set to the provided values
EnemyBullet::EnemyBullet(float x_, float y_, const EnemyBullet &prefab, float angle, float speed) :
                         Entity(x_, y_, prefab.width, prefab.height, prefab.texture),
                         _screen_width(prefab._screen_width), _screen_height(prefab._screen_height), _angle(angle), _speed(speed) {}

EnemyBullet::~EnemyBullet()
{
    texture = nullptr;
}

EnemyBullet::EnemyBullet(const EnemyBullet &source) : 
                         Entity(source.x, source.y, source.width, source.height, source.texture),
                         _screen_width(source._screen_width), _screen_height(source._screen_height), _angle(source._angle), _speed(source._speed) {}

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

EnemyBullet::EnemyBullet(EnemyBullet &&source) :
                         Entity(source.x, source.y, source.width, source.height, source.texture),
                         _screen_width(source._screen_width), _screen_height(source._screen_height), _angle(source._angle), _speed(source._speed)
{
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