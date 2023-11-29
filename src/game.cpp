#include "SDL.h"

#include <algorithm>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "playerBullet.h"
#include "enemy.h"
#include "game.h"
#include "hitbox.h"
#include "player.h"
#include "stage.h"
#include "stageState.h"
#include "spawn.h"

Game::Game(std::size_t screen_width, std::size_t screen_height)
{
    _screen_width = screen_width;
    _screen_height = screen_height;

    // Textures
    std::string textures_folder = "../Resources/Textures/";
    Game::_textures_to_load["bg"] = textures_folder + "bg.png";
    Game::_textures_to_load["enemy"] = textures_folder + "enemy.png";
    Game::_textures_to_load["enemy2"] = textures_folder + "enemy2.png";
    Game::_textures_to_load["boss"] = textures_folder + "boss.png";
    Game::_textures_to_load["enemyBullet"] = textures_folder + "enemyBullet.png";
    Game::_textures_to_load["player"] = textures_folder + "player.png";
    Game::_textures_to_load["playerBullet"] = textures_folder + "playerBullet.png";

    // Stage data
    std::string data_folder = "../Resources/Data/";
    Game::_stages_to_load.push_back(data_folder + "stage1.txt");
}

void Game::LoadResources(Renderer &renderer)
{
    renderer.LoadTextures(_textures_to_load);
}

std::deque<Spawn> Game::ReadStageDataFromFile(std::string filename)
{
    std::deque<Spawn> spawns;

    std::ifstream stage_file_stream(filename);
    if (stage_file_stream.is_open())
    {    
        std::string line;
        float time, x;
        std::string id;

        while (std::getline(stage_file_stream, line))
        {
            std::istringstream linestream(line);
            linestream >> time >> x >> id;
            spawns.push_back(Spawn{time, x, id});
        };
    }
    else
        std::cout << "Error: No stage data file! Please reinstall." << std::endl;

    return spawns;
}

void Game::Run(Controller &controller, Renderer &renderer, std::size_t target_frame_duration)
{
    Uint32 title_timestamp = SDL_GetTicks();
    
    Uint32 frame_start;
    Uint32 frame_end;

    Uint32 fps = 0;
    Uint32 fps_timestamp = SDL_GetTicks();

    Uint32 frame_duration;
    int frame_count = 0;
    _running = true;

    _player = Player(_screen_width/2, _screen_height-50, *renderer.GetTexture("player"), _screen_width, _screen_height);

    // --- Retrieve textures for first stage
    //
    std::map<std::string, SDL_Texture*> stage1_textures;
    stage1_textures["enemy"] = renderer.GetTexture("enemy");
    stage1_textures["enemy2"] = renderer.GetTexture("enemy2");
    stage1_textures["boss"] = renderer.GetTexture("boss");
    stage1_textures["enemyBullet"] = renderer.GetTexture("enemyBullet");
    stage1_textures["playerBullet"] = renderer.GetTexture("playerBullet");

    // --- Create first stage
    //
    std::deque<Spawn> spawns = ReadStageDataFromFile(_stages_to_load[0]);
    Stage current_stage(_player, _screen_width, _screen_height, stage1_textures, spawns);

    // --- Get going...
    //
    current_stage.Start();
    _time_last_update = SDL_GetTicks();

    while (_running)
    {
        frame_start = SDL_GetTicks();

        // Input, Update, Render - the main game loop.
        controller.HandleInput(_running, _player);
        Update(current_stage);
        renderer.Render(_player, current_stage);

        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render cycle
        // takes.
        frame_count++;
        frame_duration = frame_end - frame_start;

        // After every second, update the fps counter.
        if (frame_end - fps_timestamp >= 1000)
        {
            fps = frame_count;   
            fps_timestamp = frame_end;
            frame_count = 0;
        }

        if (frame_end - title_timestamp >= 200)
        {
            renderer.UpdateWindowTitle(_player.score, fps);
            title_timestamp = frame_end;
        }

        // If the time for this frame is too small (i.e. frame_duration is
        // smaller than the target ms_per_frame), delay the loop to
        // achieve the correct frame rate.
        if (frame_duration < target_frame_duration)
            SDL_Delay(target_frame_duration - frame_duration);
    }
}

void Game::Update(Stage &current_stage)
{
    Uint32 time_since_last_update = SDL_GetTicks() - _time_last_update; // ms

    StageState state = current_stage.Update(time_since_last_update);

    if (state == StageState::error || state == StageState::finished)
        _running = false;

    _time_last_update = SDL_GetTicks();
}

int Game::GetScore() const { return _player.score; }