//
// Created by satori on 10/4/26.
//
#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "GameEngine.h"

namespace Engine {
    namespace {;
        Uint64 PreviousTime;
        SDL_Event Event;
        bool PreviousKeyState[SDL_SCANCODE_COUNT];
    }

    static SDL_Window* Window = nullptr;
    static SDL_Renderer* Renderer = nullptr;
    static TTF_TextEngine* TextEngine = nullptr;
    static MIX_Mixer* Mixer = nullptr;

    static MIX_Track* BGMTrack = nullptr;

    bool Init(const int width, const int height, const std::string& title) {
        //set Initial time for delta time calculation
        PreviousTime = SDL_GetTicksNS();

        //Initialize window and shit
        bool success = true;

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            std::cout << "SDL failed to initialize: " << SDL_GetError() << std::endl;
            success = false;
        }
        else if (!SDL_CreateWindowAndRenderer(title.c_str(), width, height, SDL_WINDOW_OPENGL, &Window, &Renderer)) {
            std::cout << "SDL failed to create window or renderer: " << SDL_GetError() << std::endl;
            success = false;
        }

        SDL_SetRenderVSync(Renderer, 1);

        //Initialize audio
        if (!MIX_Init()) {
            std::cout << "MIX failed to initialize: " << SDL_GetError() << std::endl;
        }

        Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);



        //Initialize text renderer
        if (!TTF_Init()) {
            std::cout << "SDL_ttf failed to initialize: " << SDL_GetError() << std::endl;
            success = false;
        }

        TextEngine = TTF_CreateRendererTextEngine(Renderer);

        return success;
    };

    void Quit() {
        SDL_DestroyWindow(Window);
        SDL_DestroyRenderer(Renderer);
        TTF_Quit();
        MIX_Quit();
        SDL_Quit();
    }

    void GetDeltaTime() {
        const Uint64 CurrentTime = SDL_GetTicksNS();
        const Uint64 ElapsedTime = CurrentTime - PreviousTime;
        PreviousTime = CurrentTime;

        dTime = static_cast<float>(ElapsedTime) / 1000000000.0f;
    }

    void ProcessInput() {
        const bool *KeyState = SDL_GetKeyboardState(nullptr);
        for (int i = 0; i < SDL_SCANCODE_COUNT; ++i) {
            PreviousKeyState[i] = KeyState[i];
        }

        SDL_PumpEvents();

        while (SDL_PollEvent(&Event)) {
            if (Event.type == SDL_EVENT_QUIT) {
                std::cout << "Quitting..." << std::endl;
                ShouldQuit = true;
            }
        }
    }

    bool IsKeyDown(const SDL_Scancode scancode) {
        const bool *KeyState = SDL_GetKeyboardState(nullptr);
        bool isDown = false;

        if (KeyState[scancode]) {
            isDown = true;
        }

        return isDown;
    }

    bool IsKeyReleased(const SDL_Scancode scancode) {
        const bool *KeyState = SDL_GetKeyboardState(nullptr);

        return !KeyState[scancode] && PreviousKeyState[scancode];
    }

    SDL_Texture* LoadImage(const std::string& fileName) {
        SDL_Texture* texture = IMG_LoadTexture(Renderer, fileName.c_str());

        if (!texture) {
            std::cout << "Failed to load texture: " << SDL_GetError() << std::endl;
        } else {
            std::cout << "Loaded texture: " << texture << std::endl;
        }

        return texture;
    }

    TTF_Font* LoadFont(const std::string& fileName, const float fontSize) {
        TTF_Font* font = TTF_OpenFont(fileName.c_str(), fontSize);

        if (!font) {
            std::cout << "Failed to load font: " << SDL_GetError() << std::endl;
        } else {
            std::cout << "Loaded font: " << font << std::endl;
        }

        return font;
    }

    MIX_Audio* LoadSFX(const std::string& fileName) {
        MIX_Audio* audio = MIX_LoadAudio(Mixer, fileName.c_str(), true);

        if (!audio) {
            std::cout << "Failed to load audio: " << SDL_GetError() << std::endl;
        } else {
            std::cout << "Loaded audio: " << audio << std::endl;
        }

        return audio;
    }

    MIX_Audio* LoadBGM(const std::string& fileName) {
        MIX_Audio* audio = MIX_LoadAudio(Mixer, fileName.c_str(), false);

        if (!audio) {
            std::cout << "Failed to load audio: " << SDL_GetError() << std::endl;
        } else {
            std::cout << "Loaded audio: " << audio << std::endl;
        }

        return audio;
    }

    void DrawImage(SDL_Texture& Texture, const float x, const float y) {
        float width = 0.0f;
        float height = 0.0f;

        if (!SDL_GetTextureSize(&Texture, &width, &height)) {
            std::cout << "Failed to get texture size: " << SDL_GetError() << std::endl;
        }

        const SDL_FRect destRect = { .x = x, .y = y, .w = width, .h = height };

        if (!SDL_RenderTexture(Renderer, &Texture, nullptr, &destRect)) {
            std::cout << "Failed to render texture: " << SDL_GetError() << std::endl;
        }
    }

    void DrawText(const std::string& text, const float x, const float y, TTF_Font* font) {
        if (!TextEngine || !font) return;

        if (TTF_Text *TextToDraw = TTF_CreateText(TextEngine, font, text.c_str(), 0)) {
            TTF_DrawRendererText(TextToDraw, x, y);
            TTF_DestroyText(TextToDraw); // Clean up the temporary layout object
        }
    }

    void PlaySFX(MIX_Audio& audio) {

    }


    void StartRender() {
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
        SDL_RenderClear(Renderer);
    }

    void RenderFrame() {
        SDL_RenderPresent(Renderer);
    }

}
