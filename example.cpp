#include <iostream>
#include <string>
#include "GameEngine.h"

int main(int argc, char ** argv) {
    int WindowWidth = 720;
    int WindowHeight = 480;
    std::string WindowTitle = "SDL Window";

    if (!Engine::Init(WindowWidth, WindowHeight, WindowTitle)) {
        std::cout << "Failed to initialize!" << std::endl;
        return 1;
    }
    
    while (!Engine::ShouldQuit) {
        Engine::GetDeltaTime();
        Engine::ProcessInput();

        // Game logic here

        Engine::StartRender();

        // Render here
        
        Engine::RenderFrame();
    }

    Engine::Quit();

    return 0;
}
