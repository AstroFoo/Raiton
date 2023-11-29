#ifndef STAGE_H
#define STAGE_H

#include "SDL.h"

#include <condition_variable>
#include <deque>
#include <memory>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "enemy.h"
#include "enemyBullet.h"
#include "entity.h"
#include "player.h"
#include "playerBullet.h"
#include "stageState.h"
#include "spawn.h"

template<class T>
class MessageQueue
{
    public:
        T receive();
        void send(T &&item);

    private:
        std::mutex _mutex;
        std::condition_variable _cond;
        std::deque<T> _queue;
};

class Stage
{
    public:
        Stage(Player &player,
              std::size_t screen_width,
              std::size_t screen_height,
              std::map<std::string, SDL_Texture*> &textures,
              std::deque<Spawn> spawns);

        const std::vector<std::shared_ptr<Enemy>> &GetEnemies() const;
        const std::vector<std::shared_ptr<EnemyBullet>> &GetEnemyBullets() const;
        const std::vector<std::shared_ptr<PlayerBullet>> &GetPlayerBullets() const;
        ~Stage();
        void Start();
        StageState Update(Uint32 time_since_last_update);

    private:
        Player &_player;
        std::size_t _screen_width;
        std::size_t _screen_height;
        std::map<std::string, SDL_Texture*> _textures;
        std::deque<Spawn> _spawns;

        std::vector<std::shared_ptr<Enemy>> _enemies;
        std::vector<std::shared_ptr<EnemyBullet>> _enemy_bullets;
        std::vector<std::shared_ptr<PlayerBullet>> _player_bullets;

        std::map<std::string, std::shared_ptr<Enemy>> _prefab_enemies;
        std::map<std::string, std::shared_ptr<EnemyBullet>> _prefab_enemy_bullets;
        std::map<std::string, std::shared_ptr<PlayerBullet>> _prefab_player_bullets;

        Uint32 _start_time;
        Uint32 _current_time;
        Uint32 _end_time;
        Uint32 _player_death_end_time = 3000;
        Uint32 _player_victory_end_time = 5000;
        Uint32 _player_transition_time = 0;
        StageState _stage_state;

        Spawn _next_spawn;
        
        std::vector<Entity*> GetEntities();
        void UpdateEntities(Uint32 time_since_last_update);
        void ProcessCollisions();
        bool Collision(Entity &entity1, Entity &entity2);
        bool Collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
        void AddNewEntities();
        void IncrementScore();
        void RemoveOldEntities();
        
        // Concurrency code
        void SimulateOOBChecker();
        void CheckForOOBEntities();
        void SimulateOOBRemover();
        void RemoveOOBEntities();
        
        std::vector<std::thread> _threads;
        MessageQueue<bool> _entity_monitor;
        std::mutex _mutex;
};

#endif