#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "SDL_image.h"
#include "GameMap.h"
#include "Character.h"
#include "Enemy.h"
#include <memory>
#include <vector>



class Engine
{
    Engine();
    ~Engine();

    SDL_Window* m_Window {nullptr};
    SDL_Renderer* m_Renderer {nullptr};

    std::shared_ptr<GameMap> m_LevelMap;
    std::shared_ptr<Character> m_Player {nullptr};
    friend class AI;
    friend class Battle;

    std::vector<std::shared_ptr<Character>> m_Enemies;
    std::shared_ptr<EnemySpawner> m_EnemySpawner;
    friend class EnemySpawner;

    bool m_isRunning;

public:


    static Engine& getInstance();   // Engine singleton getter

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


#endif /* ENGINE_H */
