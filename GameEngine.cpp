//
// Created by satori on 10/4/26.
//
#include <iostream>
#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "GameEngine.h"

namespace Engine {
    namespace {;
        Uint64 PreviousTime; //for delta time calculation

        bool PreviousKeyState[SDL_SCANCODE_COUNT];
        Uint32 PreviousMouseState = 0;
        Uint32 CurrentMouseState = 0;

        SDL_Window* Window = nullptr;
        SDL_Renderer* Renderer = nullptr;
        SDL_Event Event;
        TTF_TextEngine* TextEngine = nullptr;

        MIX_Mixer* Mixer = nullptr;
        MIX_Track* BGMTrack = nullptr;
        std::vector<MIX_Track*> SFXPool;
    }

    namespace {
        size_t GetUTF8ByteLength(const std::string& text, const size_t maxChars) {
            if (maxChars == 0) return 0;

            const char* p = text.c_str();
            size_t remainingBytes = text.size();
            size_t chars = 0;

            while (chars < maxChars && remainingBytes > 0) {
                Uint32 codepoint = SDL_StepUTF8(&p, &remainingBytes);
                if (codepoint == 0) break;               // end of string
                // SDL_StepUTF8 already advanced p and reduced remainingBytes
                ++chars;
            }

            return text.size() - remainingBytes;
        }
    }

    bool Init(const int width, const int height, const std::string& title) {
        //set Initial time for delta time calculation
        PreviousTime = SDL_GetTicksNS();

        //Initialize window and shit
        bool success = true;

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
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

        //Create Mixer
        Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (!Mixer) {
            std::cout << "Failed to create mixer device: " << SDL_GetError() << std::endl;
        } else {
            // Create the dedicated BGM track
            BGMTrack = MIX_CreateTrack(Mixer);

            // Pre-allocate an SFX pool of 16 tracks
            for (int i = 0; i < 16; ++i) {
                if (MIX_Track* sfxTrack = MIX_CreateTrack(Mixer)) {
                    SFXPool.push_back(sfxTrack);
                } else {
                    std::cout << "Failed to create track: " << SDL_GetError() << std::endl;
                }
            }
        }

        //Initialize text renderer
        if (!TTF_Init()) {
            std::cout << "SDL_ttf failed to initialize: " << SDL_GetError() << std::endl;
            success = false;
        }

        TextEngine = TTF_CreateRendererTextEngine(Renderer);

        return success;
    };

    void Quit() {
        if (BGMTrack) MIX_DestroyTrack(BGMTrack);
        for (auto* track : SFXPool) {
            MIX_DestroyTrack(track);
        }
        SFXPool.clear();
        if (Mixer) MIX_DestroyMixer(Mixer);
        if (TextEngine) TTF_DestroyRendererTextEngine(TextEngine);

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
        //Get keyboard state
        const bool *KeyState = SDL_GetKeyboardState(nullptr);
        for (int i = 0; i < SDL_SCANCODE_COUNT; ++i) {
            PreviousKeyState[i] = KeyState[i];
        }

        //Update mouse state
        if (SDL_HasMouse()) {
            PreviousMouseState = CurrentMouseState;
            CurrentMouseState = SDL_GetMouseState(nullptr, nullptr);
        }

        //I don't know
        SDL_PumpEvents();

        //Check if the application should end
        while (SDL_PollEvent(&Event)) {
            if (Event.type == SDL_EVENT_QUIT) {
                std::cout << "Quitting..." << std::endl;
                ShouldQuit = true;
            }
        }
    }

    bool IsKeyHeld(const SDL_Scancode scancode) {
        const bool *KeyState = SDL_GetKeyboardState(nullptr);
        bool isDown = false;

        if (KeyState[scancode]) {
            isDown = true;
        }

        return isDown;
    }

    bool IsKeyPressed(const SDL_Scancode scancode) {
        const bool *KeyState = SDL_GetKeyboardState(nullptr);

        return !KeyState[scancode] && PreviousKeyState[scancode];
    }

    bool IsMouseButtonDown(const int button) {
        const Uint32 mask = SDL_BUTTON_MASK(button);
        return (CurrentMouseState & mask) != 0;
    }

    bool IsMouseButtonReleased(const int button) {
        const Uint32 mask = SDL_BUTTON_MASK(button);
        return !(CurrentMouseState & mask) && (PreviousMouseState & mask);
    }

    //Asset Loading

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

    //Asset unloading

    void UnloadImage(SDL_Texture* texture) {
        if (texture) SDL_DestroyTexture(texture);
    }

    void UnloadFont(TTF_Font* font) {
        if (font) TTF_CloseFont(font);
    }

    void UnloadAudio(MIX_Audio* audio) {
        if (audio) MIX_DestroyAudio(audio);
    }

    //Asset rendering

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

    //Strings with a character limit
    void DrawText(const std::string& text, const float x, const float y, TTF_Font* font, const size_t len) {
        if (!TextEngine || !font) return;

        const size_t textLen = (len == 0) ? 0 : GetUTF8ByteLength(text, len);

        if (TTF_Text *TextToDraw = TTF_CreateText(TextEngine, font, text.c_str(), textLen)) {
            TTF_DrawRendererText(TextToDraw, x, y);
            TTF_DestroyText(TextToDraw); // Clean up the temporary layout object
        }
    }

    void StartRender() {
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
        SDL_RenderClear(Renderer);
    }

    void RenderFrame() {
        SDL_RenderPresent(Renderer);
    }

    //Audio Playback

    void PlayBGM(MIX_Audio* audio, const bool loop) {
        if (!BGMTrack || !audio) return;
        MIX_SetTrackAudio(BGMTrack, audio);

        // Create options for infinite looping (-1)
        const SDL_PropertiesID properties = SDL_CreateProperties();
        SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, loop == true ? -1 : 0);

        MIX_PlayTrack(BGMTrack, properties);
        SDL_DestroyProperties(properties);
    }

    void PlaySFX(MIX_Audio* audio) {
        if (!audio || SFXPool.empty()) return;

        MIX_Track* targetTrack = nullptr;

        // Find an idle track in the pool
        for (auto* track : SFXPool) {
            if (!MIX_TrackPlaying(track)) {
                targetTrack = track;
                break;
            }
        }

        // Fallback: steal the first track if all are busy
        if (!targetTrack) {
            targetTrack = SFXPool[0];
        }

        MIX_SetTrackAudio(targetTrack, audio);
        MIX_PlayTrack(targetTrack, 0);
    }

    //utils

    std::string WrapText(const std::string& text, TTF_Font* font, int maxWidth)
    {
        std::string result;
        std::string line;

        size_t i = 0;
        while (i < text.size()) {
            // Find end of next word (or end of string)
            size_t wordStart = i;
            while (i < text.size() && text[i] != ' ' && text[i] != '\n') {
                // Advance one UTF-8 character
                const char* p = text.c_str() + i;
                size_t rem = text.size() - i;
                SDL_StepUTF8(&p, &rem);
                i = p - text.c_str();
            }

            std::string word = text.substr(wordStart, i - wordStart);

            // Measure current line + word
            std::string test = line.empty() ? word : line + " " + word;
            int w = 0, h = 0;
            TTF_GetStringSize(font, test.c_str(), 0, &w, &h);

            if (w > maxWidth && !line.empty()) {
                // Need to wrap
                if (!result.empty()) result += '\n';
                result += line;
                line = word;
            } else {
                if (!line.empty()) line += ' ';
                line += word;
            }

            // Skip the space
            if (i < text.size() && text[i] == ' ') ++i;
            if (i < text.size() && text[i] == '\n') {
                if (!result.empty()) result += '\n';
                result += line;
                line.clear();
                ++i;
            }
        }

        if (!line.empty()) {
            if (!result.empty()) result += '\n';
            result += line;
        }

        return result;
    }

}
