#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "player.h"

void Controller::HandleInput(bool &running, Player &player)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                running = false;
                break;
        
            case SDL_KEYDOWN:
                KeyDownProcess(&event.key, player);
                if (CheckForEscapeKey(&event.key))
                    running = false;
                break;
        
            case SDL_KEYUP:
                KeyUpProcess(&event.key, player);
                break;

            default:
                break;
        }
    }
}

bool Controller::CheckForEscapeKey(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < _max_keys)
    {
        if (event->keysym.scancode == SDL_SCANCODE_ESCAPE)
            return true;
    }
    return false;
}

void Controller::KeyDownProcess(SDL_KeyboardEvent *event, Player &player)
{
    if (event->repeat == 0 && event->keysym.scancode < _max_keys)
    {
        if (event->keysym.scancode == SDL_SCANCODE_UP)
            player.SetIsMovingUp(true);
        if (event->keysym.scancode == SDL_SCANCODE_DOWN)
            player.SetIsMovingDown(true);
        if (event->keysym.scancode == SDL_SCANCODE_LEFT)
            player.SetIsMovingLeft(true);
        if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
            player.SetIsMovingRight(true);
        if (event->keysym.scancode == SDL_SCANCODE_Z)
            player.SetIsFiring(true);
    }
}

void Controller::KeyUpProcess(SDL_KeyboardEvent *event, Player &player)
{
    if (event->repeat == 0 && event->keysym.scancode < _max_keys)
    {
        if (event->keysym.scancode == SDL_SCANCODE_UP)
            player.SetIsMovingUp(false);
        if (event->keysym.scancode == SDL_SCANCODE_DOWN)
            player.SetIsMovingDown(false);
        if (event->keysym.scancode == SDL_SCANCODE_LEFT)
            player.SetIsMovingLeft(false);
        if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
            player.SetIsMovingRight(false);
        if (event->keysym.scancode == SDL_SCANCODE_Z)
            player.SetIsFiring(false);
    }
}