#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "SDL_image.h"

#include <map>
#include <string>

#include "player.h"
#include "stage.h"

class Renderer
{
    public:
        Renderer(const std::size_t screen_width, const std::size_t screen_height);
        ~Renderer();

        void LoadTextures(std::map<std::string, std::string> textures_to_load);
        SDL_Texture *LoadTexture(std::string texture_file);
        SDL_Texture *GetTexture(std::string string);
        void Blit(const Entity &entity);
        void Blit(int x, int y, SDL_Texture *texture);
        void Render(const Player &player, const Stage &stage);
        void UpdateWindowTitle(int score, int fps);

    private:
        SDL_Window *sdl_window;
        SDL_Renderer *sdl_renderer;

        std::map<std::string, SDL_Texture*> _textures;

        const std::size_t screen_width;
        const std::size_t screen_height;
};

#endif