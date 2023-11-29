#ifndef ENEMY_H
#define ENEMY_H

#include "SDL.h"

#include <memory>
#include <vector>

#include "enemyBullet.h"
#include "entity.h"
#include "hitbox.h"
#include "shotPattern.h"

class Enemy : public Entity
{
    public:
        Enemy();
        Enemy(SDL_Texture &texture, std::size_t screen_width, std::size_t screen_height, int health, float shot_speed, ShotPattern shot_pattern);
        Enemy(float x_, float y_, const Enemy &prefab, bool adjust_position);
        ~Enemy();
        Enemy(const Enemy &source);
        Enemy &operator=(const Enemy &source);
        Enemy(Enemy &&source);
        Enemy &operator=(Enemy &&source);
        
        bool GetIsFiring();
        void SetIsFiring(bool is_firing);

        float CalculateAngle(float x2, float y2);
        bool CanFire();
        std::vector<std::shared_ptr<EnemyBullet>> FireShots(float player_centre_x, float player_centre_y, const EnemyBullet &prefab);
        Hitbox GetHitbox();
        bool IsOutOfBounds();
        void TakeDamage(int damage_taken);
        void Update(Uint32 time_elapsed);
        
    private:
        float _pi = 3.14159265;
        int _fire_time = 1200; // ms
        int _health;
        float _speed = 0.5f;
        float _shot_speed;
        ShotPattern _shot_pattern;

        std::size_t _screen_width;
        std::size_t _screen_height;

        bool _is_reloading = false;

        Uint32 _time_since_last_shot = 0;
};

#endif