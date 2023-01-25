#include <iostream>
#include "Engine.h"
#include "TextureManager.h"
#include "Knight.h"
#include "Input.h"
#include "Timer.h"
#include "MapParser.h"
#include "Camera.h"
#include "Global.h"
#include "Skeleton.h"
#include "Battle.h"



Engine::Engine()
{}

Engine::~Engine()
{}

Engine& Engine::getInstance()
{
    static Engine EngineInstance;
    return EngineInstance;
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

    if (!MapParser::getInstance().Load())
    {
        std::cout << "Failed to load the map." << std::endl;
    }

    m_LevelMap = MapParser::getInstance().GetMap("level1");

    TextureManager::getInstance().LoadTextures("../assets/Textures.xml");

    m_Player = std::make_unique<Knight>( std::make_shared<ObjParams>( "Knight", 100, 480) );
    m_EnemySpawner = EnemySpawner::getInstance();
    
    //  Spawn enemies from list keeping in mind possible exceptions
    try {
        m_EnemySpawner->SpawnEnemies("../Objects/EnemyList.xml", "level1");
    } catch(const std::string& error) {
        return m_isRunning = false;
    }

    Camera::getInstance().SetTarget(m_Player->GetPosition());
    
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
    float dt {Timer::getInstance().GetDeltaTime()};
    m_LevelMap->Update();
    Battle::getInstance().Update();
    m_Player->Update(dt);
    for (auto enemy : m_Enemies)
        enemy->Update(dt);
    m_LevelMap->UpdateFront();
    Camera::getInstance().Update();
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 20, 20, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_Renderer);

    m_LevelMap->Render();

    m_Player->Draw();
    for (auto enemy : m_Enemies)
        enemy->Draw();

    m_LevelMap->RenderFront();

    SDL_RenderPresent(m_Renderer);   
}

void Engine::Events()
{
    Input::getInstance().Listen();
}


