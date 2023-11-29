#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"

#include <memory>
#include <vector>

#include "playerBullet.h"
#include "entity.h"
#include "hitbox.h"

class Player : public Entity
{
    public:
        Player();
        Player(float x, float y, SDL_Texture &texture, std::size_t screen_width, std::size_t screen_height);
        bool GetIsMovingUp();
        void SetIsMovingUp(bool is_moving);
        bool GetIsMovingDown();
        void SetIsMovingDown(bool is_moving);
        bool GetIsMovingLeft();
        void SetIsMovingLeft(bool is_moving);
        bool GetIsMovingRight();
        void SetIsMovingRight(bool is_moving);
        bool GetIsFiring();
        void SetIsFiring(bool is_firing);

        bool CanFire();
        std::shared_ptr<PlayerBullet> FireShot(const PlayerBullet &prefab);
        Hitbox GetHitbox();
        bool IsOutOfBounds();
        void TakeDamage();
        void Update(Uint32 time_elapsed);

        int score = 0;

    private:
        SDL_Texture *_bullet_texture;

        int _fire_time = 200; // ms
        float _speed = 3.0f;

        std::size_t _screen_width;
        std::size_t _screen_height;

        bool _is_moving_up = false;
        bool _is_moving_down = false;
        bool _is_moving_left = false;
        bool _is_moving_right = false;
        bool _is_firing = false;
        bool _is_reloading = false;

        Uint32 _time_since_last_shot = 0;
};

#endif