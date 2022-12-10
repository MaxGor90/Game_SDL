#ifndef H_ENGINE
#define H_ENGINE

#include "SDL.h"
#include "SDL_image.h"
#include "GameMap.h"
#include <memory>




class Engine
{
    SDL_Window* m_Window {nullptr};
    SDL_Renderer* m_Renderer {nullptr};

    std::shared_ptr<GameMap> m_LevelMap;

    static std::shared_ptr<Engine> s_EngineInstance;
    bool m_isRunning;

public:

    Engine();
    ~Engine();

    static std::shared_ptr<Engine> getInstance();   // Engine getter

    std::shared_ptr<GameMap> getMap()   { return m_LevelMap; };
    

    bool Init();                    
    bool Clean();
    void Quit();

    void Update();
    void Render();
    void Events();

    inline bool isRunning() { return m_isRunning; }
    inline SDL_Renderer* getRenderer() { return m_Renderer; }   // Render getter

};


#endif /* H_ENGINE */
