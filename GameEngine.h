//
// Created by satori on 10/4/26.
//

#ifndef UNTITLED_GAMEENGINE_H
#define UNTITLED_GAMEENGINE_H

#include <string>
#include <SDL3/SDL.h>


void Update(float dTime);

namespace Engine {
    inline float dTime = 0.00f;
    inline bool ShouldQuit;

    bool Init(int width, int height, const std::string& title);
    void Quit();
    void GetDeltaTime();

    void ProcessInput();
    bool IsKeyDown(SDL_Scancode scancode);
    bool IsKeyReleased(SDL_Scancode scancode);

    SDL_Texture* LoadImage(const std::string& fileName);
    TTF_Font* LoadFont(const std::string& fileName, float fontSize);
    MIX_Audio* LoadSFX(const std::string& fileName);
    MIX_Audio* LoadBGM(const std::string& fileName);

    void DrawImage(SDL_Texture& Texture, float x, float y);
    void DrawText(const std::string& text, float x, float y, TTF_Font* font);
    void PlaySFX(MIX_Audio& audio);

    void StartRender();
    void RenderFrame();

}

#endif //UNTITLED_GAMEENGINE_H
