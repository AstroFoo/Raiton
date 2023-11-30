#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "playerBullet.h"
#include "enemy.h"
#include "player.h"
#include "renderer.h"

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height)
    : screen_width(screen_width),
      screen_height(screen_height)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
    else
    {
        // --- Initialise SDL_Image
        //
        int image_flags = IMG_INIT_PNG | IMG_INIT_JPG;
        int init_images = IMG_Init(image_flags);
        if ((init_images & image_flags) != image_flags)
        {
            std::cout << "IMG_Init: Failed to initialise required png and jpg support!\n";
            std::cout << "IMG_Init: " << IMG_GetError() << "\n"; 
        }

        // --- Create Window
        //
        Uint32 window_flags = SDL_WINDOW_RESIZABLE;
        sdl_window = SDL_CreateWindow("Raiton", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      screen_width, screen_height, window_flags);

        if (nullptr == sdl_window)
        {
            std::cerr << "Window could not be created.\n";
            std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
        }

        // --- Create renderer
        //
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        int renderer_flags = SDL_RENDERER_ACCELERATED;
        sdl_renderer = SDL_CreateRenderer(sdl_window, -1, renderer_flags);
        if (nullptr == sdl_renderer)
        {
            std::cerr << "Renderer could not be created.\n";
            std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        }
    }
}

Renderer::~Renderer()
{
    for (auto const& [key, value] : _textures)
       SDL_DestroyTexture(_textures[key]);
    IMG_Quit();

    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void Renderer::LoadTextures(std::map<std::string, std::string> textures_to_load)
{
    for (auto const& [key, value] : textures_to_load)
    {
        std::ifstream f(value);
        if (!f.good())
        {
            std::cout << "Error loading texture from " << value << std::endl;
            return;
        }
        _textures[key] = LoadTexture(value);
    }
}

SDL_Texture *Renderer::LoadTexture(std::string filename)
{
    char filename_c[filename.length() + 1];
    strcpy(filename_c, filename.c_str());
    
	SDL_Texture *texture = nullptr;
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename_c);
	texture = IMG_LoadTexture(sdl_renderer, filename_c);
	return texture;
}

SDL_Texture *Renderer::GetTexture(std::string key)
{
    return _textures[key];
}

void Renderer::Blit(int x, int y, SDL_Texture *texture)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(sdl_renderer, texture, NULL, &dest);
}

void Renderer::Blit(const Entity &entity)
{
    Blit(entity.x, entity.y, entity.texture);
}

void Renderer::Render(const Player &player, Stage &stage)
{
    // --- Clear screen
    //
    SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer);

    stage.mutex.lock();

    const auto &enemies = stage.GetEnemies();
    const auto &enemy_bullets = stage.GetEnemyBullets();
    const auto &player_bullets = stage.GetPlayerBullets();
    
    // --- Render assets
    //
    Blit(0, 0, _textures["bg"]);

    if (player.alive)
        Blit(player);
    for (auto &player_bullet : player_bullets)
        Blit(*player_bullet.get());
    for (auto &enemy : enemies)
        Blit(*enemy.get());
    for (auto &enemy_bullet : enemy_bullets)
        Blit(*enemy_bullet.get());

    stage.mutex.unlock();

    // --- Update Screen
    //
    SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps)
{
    std::string title{"Raiton! Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
    SDL_SetWindowTitle(sdl_window, title.c_str());
}
