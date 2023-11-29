#ifndef GAME_H
#define GAME_H

#include "SDL.h"

#include <deque>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "controller.h"
#include "player.h"
#include "renderer.h"
#include "stage.h"


class Game
{
    public:
        Game(std::size_t screen_width, std::size_t screen_height);

        void LoadResources(Renderer &renderer);
        void Run(Controller &controller, Renderer &renderer,
                std::size_t target_frame_duration);
        void Update(Stage &current_stage);
        int GetScore() const;

    private:
        Player _player;

        std::map<std::string, std::string> _textures_to_load;
        std::vector<std::string> _stages_to_load;

        std::size_t _screen_width;
        std::size_t _screen_height;

        bool _running = true;
        Uint32 _time_last_update;

        std::deque<Spawn> ReadStageDataFromFile(std::string filename);
};

#endif