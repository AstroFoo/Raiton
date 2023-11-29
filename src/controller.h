#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "player.h"

class Controller
{
    public:
        void HandleInput(bool &running, Player &player);
    private:
        int _max_keys = 350;

        bool CheckForEscapeKey(SDL_KeyboardEvent *event);
        void KeyDownProcess(SDL_KeyboardEvent *event, Player &player);
        void KeyUpProcess(SDL_KeyboardEvent *event, Player &player);
};

#endif