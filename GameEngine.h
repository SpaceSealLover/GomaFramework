//
// Created by satori on 10/4/26.
//

#ifndef UNTITLED_GAMEENGINE_H
#define UNTITLED_GAMEENGINE_H

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

namespace Engine {
    //Engine initialization & core loop
    inline float dTime = 0.00f;
    inline bool ShouldQuit;

    bool Init(int width, int height, const std::string& title);
    void Quit();
    void GetDeltaTime();
    void StartRender();
    void RenderFrame();

    //Keyboard and mouse input
    //Mouse buttons are SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT
    //Keys are SDL_SCANCODE_ with a letter (capitalized)

    inline float MouseX;
    inline float MouseY;

    void ProcessInput();
    void GetMousePosition(float& x, float& y);
    bool IsKeyHeld(SDL_Scancode scancode);
    bool IsKeyPressed(SDL_Scancode scancode);

    bool IsMouseButtonDown(int button);
    bool IsMouseButtonReleased(int button);

    //asset loading & unloading
    SDL_Texture* LoadImage(const std::string& fileName);
    TTF_Font* LoadFont(const std::string& fileName, float fontSize);
    MIX_Audio* LoadSFX(const std::string& fileName);
    MIX_Audio* LoadBGM(const std::string& fileName);

    void UnloadImage(SDL_Texture* texture);
    void UnloadAudio(MIX_Audio* audio);
    void UnloadFont(TTF_Font* font);

    //asset rendering and audio playback
    void DrawImage(SDL_Texture& Texture, float x, float y);
    void DrawText(const std::string& text, float x, float y, TTF_Font* font);
    void PlaySFX(MIX_Audio* audio);
    void PlayBGM(MIX_Audio* audio, bool loop);
}

#endif //UNTITLED_GAMEENGINE_H
