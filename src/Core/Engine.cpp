#include <iostream>
#include "Engine.h"
#include "TextureManager.h"
#include "Knight.h"
#include "Input.h"
#include "Timer.h"
#include "MapParser.h"
#include "Camera.h"
#include "Global.h"
#include "Enemy.h"





std::shared_ptr<Engine> Engine::s_EngineInstance {nullptr};
std::unique_ptr<Knight> Player {nullptr};
std::unique_ptr<Enemy> Skeleton {nullptr};


Engine::Engine()
{}

Engine::~Engine()
{}

std::shared_ptr<Engine> Engine::getInstance()
{
    //  One instance of Engine must be running
    return s_EngineInstance = (s_EngineInstance == nullptr)? std::make_shared<Engine>() : s_EngineInstance;
}

bool Engine::Init()
{
    if( (SDL_Init(SDL_INIT_VIDEO) != 0) && (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) )
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    SDL_WindowFlags windowFlags { SDL_WindowFlags(SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE) };
    m_Window = SDL_CreateWindow("Game_SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags); 
    
    if( m_Window == nullptr )
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if( m_Renderer == nullptr )
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (!MapParser::GetInstance()->Load())
    {
        std::cout << "Failed to load the map." << std::endl;
    }

    m_LevelMap = MapParser::GetInstance()->GetMap("Level1");

    TextureManager::getInstance()->LoadTextures("../assets/Textures.xml");

    // TextureManager::getInstance()->Load("Knight", "../assets/Knight/Knight.png",  200, 106);

    Player = std::make_unique<Knight>( std::make_shared<ObjParams>( "Knight", 100, 100) );

    Camera::getInstance()->SetTarget(Player->GetPosition());
    
    return m_isRunning = true;
}

bool Engine::Clean()
{
    return true;
}

void Engine::Quit()
{
    m_isRunning = false;
}

void Engine::Update()
{
    float dt {Timer::getInstance()->GetDeltaTime()};
    m_LevelMap->Update();
    Player->Update(dt);
    m_LevelMap->UpdateFront();
    Camera::getInstance()->Update(dt);
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 20, 20, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_Renderer);

    m_LevelMap->Render();

    Player->Draw();

    m_LevelMap->RenderFront();

    SDL_RenderPresent(m_Renderer);   
}

void Engine::Events()
{
    Input::getInstance()->Listen();
}


