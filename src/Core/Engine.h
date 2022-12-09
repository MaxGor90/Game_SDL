#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "SDL_image.h"
#include "GameMap.h"




class Engine
{
    Engine();
    ~Engine();

    SDL_Window* m_Window {nullptr};
    SDL_Renderer* m_Renderer {nullptr};

    GameMap* m_LevelMap;

    static Engine* s_EngineInstance;
    bool m_isRunning;

public:

    static Engine* getInstance();   // Engine getter

    GameMap* getMap()   { return m_LevelMap; };
    

    bool Init();                    
    bool Clean();
    void Quit();

    void Update();
    void Render();
    void Events();

    inline bool isRunning() { return m_isRunning; }
    inline SDL_Renderer* getRenderer() { return m_Renderer; }   // Render getter

};


#endif /* ENGINE_H */
