#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "GameEngine.h"

int main(int argc, char ** argv) {
    constexpr int WindowWidth = 720;
    constexpr int WindowHeight = 480;
    const std::string WindowTitle = "SDL Window";

    if (!Engine::Init(WindowWidth, WindowHeight, WindowTitle)) {
        std::cout << "Failed to initialize!" << std::endl;
        return 1;
    }

    SDL_Texture *image = Engine::LoadImage("Assets/10003779803.jpg");
    float x = 0.0f;
    float y = 0.0f;

    TTF_Font *font = Engine::LoadFont("Assets/MadouFutoMaruGothic-d9Xo7.ttf", 18.0f);

    while (!Engine::ShouldQuit) {
        Engine::GetDeltaTime();
        Engine::ProcessInput();

        if (Engine::IsKeyDown(SDL_SCANCODE_D)) {
            x += 64.0f * Engine::dTime;
        }

        if (Engine::IsKeyDown(SDL_SCANCODE_A)) {
            x -= 64.0f * Engine::dTime;
        }

        if (Engine::IsKeyDown(SDL_SCANCODE_W)) {
            y -= 64.0f * Engine::dTime;
        }

        if (Engine::IsKeyDown(SDL_SCANCODE_S)) {
            y += 64.0f * Engine::dTime;
        }

        if (Engine::IsKeyReleased(SDL_SCANCODE_F)) {
            y += 64.0f;
        }

        Engine::StartRender();

        if (!Engine::IsKeyDown(SDL_SCANCODE_Z)) {
            Engine::DrawImage(*image, x, y);
        }

        std::string FPSCount = "FPS: " + std::to_string(static_cast<int>(1.0f / Engine::dTime));
        Engine::DrawText(FPSCount, 0, 0, font);

        Engine::RenderFrame();
    }

    Engine::Quit();

    return 0;
}