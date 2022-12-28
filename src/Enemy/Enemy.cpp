#include "Enemy.h"
#include "Skeleton.h"
#include "SkeletonTest.h"
#include "Engine.h"
#include "tinyxml2.h"
#include "SDL.h"



Enemy::Enemy(std::shared_ptr<ObjParams> params) :
    Character::Character(params)
{
    m_Animation = std::make_unique<Animation>();

    m_RigidBody = std::make_unique<RigidBody>();
    
    m_Collision = Collision::GetInstance();
}


Enemy::~Enemy()
{
    Clean();
}


void Enemy::Draw()
{
    m_Animation->Draw(m_Transform->m_X, m_Transform->m_Y);
}

void Enemy::Update(float dt)
{
    switch (m_Condition)
    {
    case Attacking:
        Attack(dt);
        return;
    case Rolling:
        Roll(dt);
        return;
    case Jumping:
        Jump(dt);
        return;
    case Falling:
        Fall(dt);
        return;
    case Running:
        Run(dt);
        return;
    case IsIdle:
        Idle(dt);
        return;
    case Blocking:
        Block();
        return;
    default:
        break;
    }
}

void Enemy::Clean()
{
    
}

void Enemy::Idle(float dt) 
{
    CheckDirectionSetParams(m_AnimationSequences.at("idle"));         // Idle

    m_RigidBody->UnsetForce();
    m_RigidBody->Stop();

    m_ComboState = NO;

    CheckCollisions();
    m_Animation->UpdateCycle();
}

void Enemy::Run(float dt) 
{
    m_RigidBody->UnsetForce();
    CheckDirectionSetParams(m_AnimationSequences.at("run"));                         // Run

    m_RigidBody->Update(dt);

    CheckCollisions();

    m_Animation->UpdateCycle();

}


std::shared_ptr<EnemySpawner> EnemySpawner::EnemySpawnerInstance {nullptr};

void EnemySpawner::SpawnEnemies(const std::string& enemyList, const std::string& levelName) 
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(enemyList.c_str());

    if (doc.Error())
    {
        SDL_Log("%s: Failed to load the document %s: %s", SDL_FUNCTION, enemyList.c_str(), doc.ErrorStr());
        return;
    }

    tinyxml2::XMLElement* root { doc.RootElement() };

    enemyType type;
    std::string texureID;
    float x, y;
    SDL_RendererFlip flip {SDL_FLIP_NONE};
    
    for (tinyxml2::XMLElement* map { root->FirstChildElement("map")}; map != nullptr; map = map->NextSiblingElement("map"))
    {
        // Finding our level
        std::string levelNameStr = map->Attribute("name");
        if (levelNameStr != levelName)
            continue;
        
        // Going through all enemies on that level
        for (tinyxml2::XMLElement* enemy { map->FirstChildElement("enemy")}; enemy != nullptr; enemy = enemy->NextSiblingElement("enemy"))
        {
            std::string enemyTypeStr = enemy->Attribute("type");
            type = EnemyTypeMap.at(enemyTypeStr);
            texureID = enemy->Attribute("textureID");
            x = enemy->FloatAttribute("spawnX");
            y = enemy->FloatAttribute("spawnY");

            if (enemy->FindAttribute("flip") != nullptr)
            {
                std::string flipStr = enemy->Attribute("flip");
                if (flipStr == "hor")
                    flip = SDL_FLIP_HORIZONTAL;
                else if (flipStr == "vert")
                    flip = SDL_FLIP_VERTICAL;
            }

            // If there's no needed factory in map yet - add one
            if (m_Factories.find(type) == m_Factories.end())
            {
                switch (type)
                {
                case skeleton_melee:
                    m_Factories[type] = std::shared_ptr<SkeletonFactory>(new SkeletonFactory());
                    break;
                case skeleton_test:
                    m_Factories[type] = std::shared_ptr<SkeletonTestFactory>(new SkeletonTestFactory());
                    break;
                default:
                    break; 
                }
            }

            // Spawning enemies directly into m_Enemies vector of Engine class 
            Engine::getInstance()->m_Enemies.push_back(
                m_Factories.at(type)->Spawn(std::make_shared<ObjParams>(texureID, x, y, flip)) );
        }
    }
    return;
}


