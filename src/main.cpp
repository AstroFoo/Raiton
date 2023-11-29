#include <iostream>
#include "userSettings.h"
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argc, char* args[])
{
    // Read user settings file for basic program information

    std::cout << "*** Welcome to Raiton - A shoot 'em up for the Udacity C++ training course ***" << std::endl;
    std::cout << std::endl;

    std::cout << "Reading screen size..." << std::endl;
    
    UserSettings userSettings;
    userSettings.ReadUserSettingsFile();

    constexpr int target_FPS{60};
    constexpr int target_frame_duration{1000 / target_FPS};
    const int screen_width{userSettings.GetScreenWidth()};
    const int screen_height{userSettings.GetScreenHeight()};

    Renderer renderer(screen_width, screen_height);
    Controller controller;

    Game game(screen_width, screen_height);

    std::cout << "Loading resources..." << std::endl;
    game.LoadResources(renderer);
    game.Run(controller, renderer, target_frame_duration);
    
    std::cout << "Thanks for playing!\n";
    std::cout << "Your score is: " << game.GetScore() << "\n";

    return 0;
}