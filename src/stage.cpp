#include "SDL.h"

#include <algorithm>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "enemyBullet.h"
#include "player.h"
#include "playerBullet.h"
#include "shotPattern.h"
#include "stage.h"
#include "stageState.h"
#include "spawn.h"

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_queue.empty(); });

    T message = std::move(_queue.back());
    _queue.pop_back();

    return message;
}

template <typename T>
void MessageQueue<T>::send(T &&message)
{
    std::lock_guard<std::mutex> uLock(_mutex);
    _queue.push_back(std::move(message));
    _cond.notify_one();
}


Stage::Stage(Player &player,
             std::size_t screen_width,
             std::size_t screen_height,
             std::map<std::string, SDL_Texture*> &textures,
             std::deque<Spawn> spawns) :
             _player(player),
             _screen_width(screen_width),
             _screen_height(screen_height),
             _spawns(spawns)
{
    // --- Generate stage prefabs
    //
    _prefab_enemies["enemy1"] = std::make_shared<Enemy>(*textures["enemy"], _screen_width, _screen_height, 3, 4.0, ShotPattern::aimed_1);
    _prefab_enemies["enemy2"] = std::make_shared<Enemy>(*textures["enemy"], _screen_width, _screen_height, 3, 5.0, ShotPattern::straight_1);
    _prefab_enemies["enemy3"] = std::make_shared<Enemy>(*textures["enemy2"], _screen_width, _screen_height, 5, 5.0, ShotPattern::straight_3);
    _prefab_enemies["enemy4"] = std::make_shared<Enemy>(*textures["enemy2"], _screen_width, _screen_height, 5, 4.0, ShotPattern::aimed_3);
    _prefab_enemies["boss1"] = std::make_shared<Enemy>(*textures["boss"], _screen_width, _screen_height, 30, 2.0, ShotPattern::star_16);
    _prefab_enemy_bullets["enemyBullet"] = std::make_shared<EnemyBullet>(*textures["enemyBullet"], _screen_width, _screen_height);
    _prefab_player_bullets["playerBullet"] = std::make_shared<PlayerBullet>(*textures["playerBullet"]);

    // Start a thread for RemoveOldEntities()
    _stage_state = StageState::running;
    std::thread t1 = std::thread(&Stage::SimulateOOBChecker, this);
    std::thread t2 = std::thread(&Stage::SimulateOOBRemover, this);
    _threads.emplace_back(std::move(t1));
    _threads.emplace_back(std::move(t2));
}

Stage::~Stage()
{
    _stage_state = StageState::finished;
    for (auto &thread : _threads)
        thread.join();
}

const std::vector<std::shared_ptr<Enemy>> &Stage::GetEnemies() const
{
    return _enemies;
}

const std::vector<std::shared_ptr<EnemyBullet>> &Stage::GetEnemyBullets() const
{
    return _enemy_bullets;
}

const std::vector<std::shared_ptr<PlayerBullet>> &Stage::GetPlayerBullets() const
{
    return _player_bullets;
}

void Stage::Start()
{
    _start_time = 0;
    _current_time = 0;
    _player_transition_time = 0;
    _next_spawn = _spawns[0];
    _stage_state = StageState::running;
}

StageState Stage::Update(Uint32 time_since_last_update)
{
    // Just on the off-chance someone runs the game for 50 days straight...
    if (_current_time > 4294967295 - time_since_last_update)
    {
        std::cout << "Error: stage current time overflow!";
        _stage_state = StageState::error;
        return _stage_state;
    }
    _current_time += time_since_last_update;
    
    if ((!_player.alive) || (_spawns.size() == 0 && _enemies.size() == 0))
        _player_transition_time += time_since_last_update;
    
    UpdateEntities(time_since_last_update);
    ProcessCollisions();
    AddNewEntities();
    IncrementScore();
    RemoveOldEntities();

    if ((_spawns.size() == 0 && _enemies.size() == 0) && _player_transition_time > _player_victory_end_time)
        _stage_state = StageState::finished;
    if (!_player.alive && _player_transition_time > _player_death_end_time)
        _stage_state = StageState::finished;
    
    return _stage_state;
}

void Stage::UpdateEntities(Uint32 time_since_last_update)
{
    _player.Update(time_since_last_update);
    for (auto entity : GetEntities())
        entity->Update(time_since_last_update);
}

void Stage::ProcessCollisions()
{
    for (auto &enemy : _enemies)
    {
        if (_player.alive)
        {
            // collision between enemy and player
            if (Collision(*enemy.get(), _player))
            {
                enemy->TakeDamage(10);
                _player.alive = false;
            }
        }

        // collision between enemy and player bullet
        for (auto &player_bullet : _player_bullets)
        {
            if (Collision(*enemy.get(), *player_bullet.get()))
            {
                enemy->TakeDamage(player_bullet->GetDamage());
                player_bullet->alive = false;
                _player.score += 10;
            }
        }
    }

    // collision between enemy bullet and player.
    if (_player.alive)
    {
        for (auto &enemy_bullet : _enemy_bullets)
        {
            if (Collision(_player, *enemy_bullet.get()))
            {
                _player.alive = false;
                enemy_bullet->alive = false;
            }
        }
    }
}

void Stage::AddNewEntities()
{
    // Player bullets
    if (_player.alive && _player.CanFire())
    {
        _player_bullets.emplace_back(_player.FireShot(*_prefab_player_bullets["playerBullet"].get()));
    }

    // Enemy bullets
    for (auto &enemy : _enemies)
    {
        if (enemy->CanFire())
        {
            float player_centre_x = _player.GetCentreX();
            float player_centre_y = _player.GetCentreY();
            std::vector<std::shared_ptr<EnemyBullet>> enemy_bullets = enemy->FireShots(player_centre_x, player_centre_y, *_prefab_enemy_bullets["enemyBullet"].get());

            for (auto &bullet : enemy_bullets)
                _enemy_bullets.emplace_back(std::move(bullet));
        }
    }
    
    while (_spawns.size() > 0 && _current_time >= (Uint32)(_next_spawn.time*1000.0))
    {
        int enemy_x = (int)(_next_spawn.x*_screen_width);
        std::string enemy_id = _next_spawn.id;

        // Add enemy
        std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(enemy_x, 0, *_prefab_enemies[enemy_id].get(), true);
        _enemies.emplace_back(std::move(enemy));
        _spawns.pop_front();

        if (_spawns.size() > 0)
            _next_spawn = _spawns[0];
    }
}

void Stage::IncrementScore()
{
    // Increment score for destroyed enemies
    for (auto &enemy : _enemies)
    {
        if (!enemy->alive)
            _player.score += 100;
    }
}

// Use erase-remove idiom described in these pages:
// https://en.cppreference.com/w/cpp/algorithm/remove
// https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
template <typename T>
void ReduceOutOfBounds(std::vector<T> &vec)
{
    vec.erase(std::remove_if(vec.begin(), vec.end(), [](T &element){ return element->IsOutOfBounds(); }), vec.end());
} 

template <typename T>
void ReduceDestroyed(std::vector<T> &vec)
{
    vec.erase(std::remove_if(vec.begin(), vec.end(), [](T &element){ return !element->alive; }), vec.end());
}

void Stage::RemoveOldEntities()
{
    ReduceDestroyed<std::shared_ptr<PlayerBullet>>(_player_bullets);
    ReduceDestroyed<std::shared_ptr<Enemy>>(_enemies);
    ReduceDestroyed<std::shared_ptr<EnemyBullet>>(_enemy_bullets);
}


std::vector<Entity*> Stage::GetEntities()
{
    std::vector<Entity*> entities;
    for (auto &player_bullet : _player_bullets)
        entities.push_back(player_bullet.get());
    for (auto &enemy : _enemies)
        entities.push_back(enemy.get());
    for (auto &enemy_bullet : _enemy_bullets)
        entities.push_back(enemy_bullet.get());
    return entities;
}

bool Stage::Collision(Entity &entity1, Entity &entity2)
{
    Hitbox hitbox1 = entity1.GetHitbox();
    Hitbox hitbox2 = entity2.GetHitbox();
    return Collision(hitbox1.x, hitbox1.y, hitbox1.width, hitbox1.height,
                     hitbox2.x, hitbox2.y, hitbox2.width, hitbox2.height);
}

bool Stage::Collision(int x1, int y1, int w1, int h1,
                     int x2, int y2, int w2, int h2)
{
    return (std::max(x1, x2) < std::min(x1 + w1, x2 + w2)) && (std::max(y1, y2) < std::min(y1 + h1, y2 + h2));
}

// --- Concurrency code
//
void Stage::SimulateOOBChecker()
{
    while (_stage_state == StageState::running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        CheckForOOBEntities();
    }
    // Send one last message to make sure threads can join
    _entity_monitor.send(false);
}

void Stage::CheckForOOBEntities()
{
    std::unique_lock<std::mutex> lck(_mutex);

    for (auto entity : GetEntities())
    {
        if (entity->IsOutOfBounds())
        {
            _entity_monitor.send(true);
            return;
        }
    }
    _entity_monitor.send(false);
}

void Stage::SimulateOOBRemover()
{
    while (_stage_state == StageState::running)
    {
        if (_entity_monitor.receive())
            RemoveOOBEntities();
    }
}

void Stage::RemoveOOBEntities()
{
    std::unique_lock<std::mutex> lck(_mutex);
    ReduceOutOfBounds<std::shared_ptr<Enemy>>(_enemies);
    ReduceOutOfBounds<std::shared_ptr<EnemyBullet>>(_enemy_bullets);
    ReduceOutOfBounds<std::shared_ptr<PlayerBullet>>(_player_bullets);
}