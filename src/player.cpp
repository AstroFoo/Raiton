#include "SDL.h"

#include <iostream>
#include <memory>

#include "player.h"
#include "playerBullet.h"

Player::Player(){}

Player::Player(float x_, float y_, SDL_Texture &texture_,
               std::size_t screen_width, std::size_t screen_height)
{
    x = x_;
    y = y_;
    texture = &texture_;

    _screen_width = screen_width;
    _screen_height = screen_height;
    
    // Derived parameters
    int width_, height_;
    SDL_QueryTexture(&texture_, NULL, NULL, &width_, &height_);
    width = width_;
    height = height_;
} 

bool Player::GetIsMovingUp() { return _is_moving_up; }
void Player::SetIsMovingUp(bool is_moving) { _is_moving_up = is_moving; }
bool Player::GetIsMovingDown() { return _is_moving_down; }
void Player::SetIsMovingDown(bool is_moving) { _is_moving_down = is_moving; }
bool Player::GetIsMovingLeft() { return _is_moving_left; }
void Player::SetIsMovingLeft(bool is_moving) { _is_moving_left = is_moving; }
bool Player::GetIsMovingRight() { return _is_moving_right; }
void Player::SetIsMovingRight(bool is_moving) { _is_moving_right = is_moving; }
bool Player::GetIsFiring() { return _is_firing; }
void Player::SetIsFiring(bool is_firing) { _is_firing = is_firing; }

bool Player::CanFire()
{
    if (_time_since_last_shot == 0 && _is_firing)
        return true;
    return false;
}

std::shared_ptr<PlayerBullet> Player::FireShot(const PlayerBullet &prefab)
{
    _time_since_last_shot = _fire_time;
    float bullet_x_offset = width/2 - prefab.width/2;
    std::shared_ptr<PlayerBullet> bullet = std::make_shared<PlayerBullet>(x + bullet_x_offset, y, prefab);
    return std::move(bullet);
}

Hitbox Player::GetHitbox()
{
    Hitbox hitbox;
    hitbox.x = x + 3;
    hitbox.y = y + 3;
    hitbox.width = width - 6;
    hitbox.height = height - 6;
    return hitbox;
}

bool Player::IsOutOfBounds()
{
    return false;
}

void Player::Update(Uint32 time_elapsed)
{
    if (_time_since_last_shot < time_elapsed)
        _time_since_last_shot = 0;
    else if (_time_since_last_shot > 0)
        _time_since_last_shot -= time_elapsed;

    if (_is_moving_up)
    {
        y -= _speed;
        if (y < 0)
            y = 0;
    }
    if (_is_moving_down)
    {
        y += _speed;
        if (y + height > _screen_height)
            y = _screen_height - height;
    }
    if (_is_moving_left)
    {
        x -= _speed;
        if (x < 0)
            x = 0;
    }
    if (_is_moving_right)
    {
        x += _speed;
        if (x + width > _screen_width)
            x = _screen_width - width;
    }
}