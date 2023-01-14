#ifndef H_ENGINE
#define H_ENGINE

#include "SDL.h"
#include "SDL_image.h"
#include "GameMap.h"
#include "Character.h"
#include "Enemy.h"
#include <memory>
#include <vector>



class Engine
{
    SDL_Window* m_Window {nullptr};
    SDL_Renderer* m_Renderer {nullptr};

    std::shared_ptr<GameMap> m_LevelMap;
    std::shared_ptr<Character> m_Player {nullptr};

    std::vector<std::shared_ptr<Character>> m_Enemies;
    std::shared_ptr<EnemySpawner> m_EnemySpawner;
    friend class EnemySpawner;

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
    inline std::shared_ptr<Character> getPalyer() { return m_Player; }
};


#endif /* H_ENGINE */
