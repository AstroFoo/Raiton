#include "SDL.h"

#include <iostream>
#include <memory>

#include "enemy.h"
#include "enemyBullet.h"
#include "entity.h"
#include "hitbox.h"
#include "shotPattern.h"

Enemy::Enemy() : Entity(),
                 _screen_width{0}, _screen_height{0}, _health{0}, _shot_speed{0}, _shot_pattern{ShotPattern::none} {}

// Constructor for prefabs
Enemy::Enemy(SDL_Texture &texture_, std::size_t screen_width, std::size_t screen_height, int health, float shot_speed, ShotPattern shot_pattern) :
             Entity(0, 0, 0, 0, &texture_),
              _screen_width{screen_width}, _screen_height{screen_height}, _health{health}, _shot_speed{shot_speed}, _shot_pattern{shot_pattern}
{
    // Derived parameters
    int width_, height_;
    SDL_QueryTexture(&texture_, NULL, NULL, &width_, &height_);
    width = width_;
    height = height_;
} 

// Constructor to create enemies from a prefab
Enemy::Enemy(float x_, float y_, const Enemy &prefab, bool adjust_position) :
             Enemy(prefab)
{
    x = x_;
    y = y_;

    if (adjust_position)
    {
        x -= width/2;
        y -= height;
    }
} 

Enemy::~Enemy(){}

Enemy::Enemy(const Enemy &source) :
             Entity(source.x, source.y, source.width, source.height, source.texture),
             _screen_width(source._screen_width), _screen_height(source._screen_height), _health(source._health), _shot_speed(source._shot_speed), _shot_pattern(source._shot_pattern) {}

Enemy &Enemy::operator=(const Enemy &source)
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
    _health = source._health;
    _shot_speed = source._shot_speed;
    _shot_pattern = source._shot_pattern;

    return *this;
}

Enemy::Enemy(Enemy &&source) :
             Entity(source.x, source.y, source.width, source.height, source.texture),
             _screen_width(source._screen_width), _screen_height(source._screen_height), _health(source._health), _shot_speed(source._shot_speed), _shot_pattern(source._shot_pattern)
{
    source.texture = nullptr;
}

Enemy &Enemy::operator=(Enemy &&source)
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
    _health = source._health;
    _shot_speed = source._shot_speed;
    _shot_pattern = source._shot_pattern;

    source.texture = nullptr;

    return *this;
};

float Enemy::CalculateAngle(float x2, float y2)
{
    float x1 = GetCentreX();
    float y1 = GetCentreY();
    // Check for horizontal
    if (abs(y2 - y1) < 0.001)
    {
        if (x2 > x1)
            return _pi/4;
        else
            return -_pi/4;
    }
    else
    {
        float angle = atan((x2-x1)/(y2-y1));
        if (y2 < y1)
            angle += _pi;
        return angle;
    }
}

bool Enemy::CanFire()
{
    if (_time_since_last_shot == 0)
        return true;
    return false;
}

std::vector<std::shared_ptr<EnemyBullet>> Enemy::FireShots(float player_centre_x, float player_centre_y, const EnemyBullet &prefab)
{
    _time_since_last_shot = _fire_time;

    float bullet_x_offset = width/2 - prefab.width/2;
    float bullet_y_offset = height/2 - prefab.height/2;

    std::vector<std::shared_ptr<EnemyBullet>> new_bullets;
    switch (_shot_pattern)
    {
        case ShotPattern::straight_1:
        {
            new_bullets.emplace_back(std::move(std::make_shared<EnemyBullet>(x + bullet_x_offset, y + bullet_y_offset, prefab, 0, _shot_speed)));
            break;
        }   
        case ShotPattern::straight_3:
        {
            for (int i = 0; i < 3; i++)
            {
                float deviation = 15;
                float offset = deviation*(i-1);
                new_bullets.emplace_back(std::move(std::make_shared<EnemyBullet>(x + bullet_x_offset + offset, y + bullet_y_offset, prefab, 0, _shot_speed)));
            }
            break;
        }
        case ShotPattern::aimed_1:
        {
            float angle = CalculateAngle(player_centre_x, player_centre_y);
            std::shared_ptr<EnemyBullet> bullet = std::make_shared<EnemyBullet>(x + bullet_x_offset, y + bullet_y_offset, prefab, angle, _shot_speed);
            new_bullets.emplace_back(std::move(bullet));
            break;
        }
        case ShotPattern::aimed_3:
        {
            float angle = CalculateAngle(player_centre_x, player_centre_y);
            for (int i = 0; i < 3; i++)
            {
                float deviation = 0.3;
                float offset = deviation*(i-1);
                std::shared_ptr<EnemyBullet> bullet = std::make_shared<EnemyBullet>(x + bullet_x_offset, y + bullet_y_offset, prefab, angle + offset, _shot_speed);
                new_bullets.emplace_back(std::move(bullet));
            }
            break;
        }
        case ShotPattern::star_16:
        {
            for (int i = 0; i < 16; i++)
            {
                float angle = _pi/8*(i-1);
                std::shared_ptr<EnemyBullet> bullet = std::make_shared<EnemyBullet>(x + bullet_x_offset, y + bullet_y_offset, prefab, angle, _shot_speed);
                new_bullets.emplace_back(std::move(bullet));
            }
            break;
        }
    }
    return new_bullets;
}

Hitbox Enemy::GetHitbox()
{
    Hitbox hitbox;
    hitbox.x = x;
    hitbox.y = y;
    hitbox.width = width;
    hitbox.height = height;
    return hitbox;
}

bool Enemy::IsOutOfBounds()
{
    return y > _screen_height;
}

void Enemy::TakeDamage(int damage_taken)
{
    _health -= damage_taken;
    if (_health <= 0)
        alive = false;
}

void Enemy::Update(Uint32 time_elapsed)
{
    if (_time_since_last_shot < time_elapsed)
        _time_since_last_shot = 0;
    else if (_time_since_last_shot > 0)
        _time_since_last_shot -= time_elapsed;

    y += _speed;
}