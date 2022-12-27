#include <iostream>
#include <stdexcept>
#include "Character.h"
#include "tinyxml2.h"
#include "Engine.h"


Character::Character(std::shared_ptr<ObjParams> params) :
    GameObject(params)
{
    m_CollisionBox = std::make_shared<SDL_Rect>( SDL_Rect {0,0,0,0} );
    m_CollisionBoxAtk = std::make_shared<SDL_Rect>( SDL_Rect {0,0,0,0} );
    std::string path;
    try {
        path = GetAnimParamsSource();
    } 
    catch(const std::string& error) {
        throw error;
    };
    LoadAnimations( path );
}

void Character::LoadAnimations(const std::string& fileSource)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(fileSource.c_str());

    if (doc.Error())
    {
        std::cerr << "Failed to load the document:" << fileSource << std::endl;
        return;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    
    tinyxml2::XMLElement* animations = root->FirstChildElement("animations");

    for (tinyxml2::XMLElement* el = animations->FirstChildElement("animation"); el != nullptr; el = el->NextSiblingElement("animation"))
    {
        std::shared_ptr<AnimationSequence> animSeq = AnimationParser::ParseAnimationSequence(el);
        std::string name = animSeq->getName();
        m_AnimationSequences[name] = animSeq;
    }
}

std::string Character::GetAnimParamsSource()
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile("../assets/Textures.xml");

    if (doc.Error())
    {
        throw std::string("Failed to load the document: ../assets/Textures.xml");
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    for (tinyxml2::XMLElement* el = root->FirstChildElement(); el != nullptr; el = el->NextSiblingElement())
    {
        if (el->Attribute("id") == m_TextureID)
        {    
            std::string path {el->Attribute("animParams")}; 
            return path;
        }
    } 
    throw std::string("No needed data in file ../assets/Textures.xml");
}

void Character::SetParams(std::shared_ptr<AnimationSequence> animSequence, SDL_RendererFlip flip)
{
    m_Animation->SetParams(m_TextureID, animSequence, flip);
    if (animSequence->getCollisionBox() != nullptr)
        m_CollisionBoxInsideFrame = animSequence->getCollisionBox();
    
    m_CollisionBoxAtkInsideFrame = animSequence->getAttackCollisionBox();
    m_Width = TextureManager::getInstance()->GetTextureWidth(m_TextureID);
    m_Height = TextureManager::getInstance()->GetTextureHeight(m_TextureID);

    CollisionBoxRecalc();
}

//  Updates player's collision box
void Character::CollisionBoxRecalc()
{
    m_CollisionBox->x = (int)m_Transform->m_X + m_CollisionBoxInsideFrame->x;
    m_CollisionBox->y = (int)m_Transform->m_Y + m_CollisionBoxInsideFrame->y;
    m_CollisionBox->w = m_CollisionBoxInsideFrame->w;
    m_CollisionBox->h = m_CollisionBoxInsideFrame->h;
}

void Character::CheckPosition()
{
    //  Check if Collision Box is going out of screen (and hence gameMap) bounds
    //  If yes - keep it within bounds
    if (m_Transform->m_X < -m_CollisionBoxInsideFrame->x)  m_Transform->m_X = -m_CollisionBoxInsideFrame->x;
    if (m_Transform->m_Y < -m_CollisionBoxInsideFrame->y)  m_Transform->m_Y = -m_CollisionBoxInsideFrame->y;
    if (m_Transform->m_X > Engine::getInstance()->getMap()->getMapLength() - m_Width + m_CollisionBoxInsideFrame->x)  
        m_Transform->m_X = Engine::getInstance()->getMap()->getMapLength() - m_Width + m_CollisionBoxInsideFrame->x;
    if (m_Transform->m_Y > SCREEN_HEIGHT - m_Height)  m_Transform->m_Y = SCREEN_HEIGHT - m_Height;
}

//  Recalc (with bounds check) character's position and its collision box
void Character::PositionRecalc()
{
    CheckPosition();
    CollisionBoxRecalc();
    
    m_Position->X = m_Transform->m_X + m_Width / 2;
    m_Position->Y = m_Transform->m_Y + m_Height / 2;
}

//  Updates character's attack collision box
void Character::CollisionBoxAtkRecalc()
{
    std::string animID;
    switch (m_ComboState)
    {
    case HIT_1:
        animID = "attack1";
        break;
    case HIT_2:
        animID = "attack2";
        break;
    case HIT_3:
        animID = "attack3";
        break;
    case NO:
    default:
        return;
    }
    
    int atkColBoxX = m_AnimationSequences.at(animID)->getAttackCollisionBox()->x;
    int atkColBoxWidth = m_AnimationSequences.at(animID)->getAttackCollisionBox()->w;
    
    switch (m_Direction)
    {
    case forward:
        m_CollisionBoxAtk->x = (int)m_Transform->m_X + m_CollisionBoxAtkInsideFrame->x;
        m_CollisionBoxAtk->y = (int)m_Transform->m_Y + m_CollisionBoxAtkInsideFrame->y;         
        return;
    case backward:
        m_CollisionBoxAtk->x = (int)m_Transform->m_X + m_Width - atkColBoxX - atkColBoxWidth;
        m_CollisionBoxAtk->y = (int)m_Transform->m_Y + m_CollisionBoxAtkInsideFrame->y;         
        return;
    }
}


//  Sets passed animation parameters depending on current direction
void Character::CheckDirectionSetParams(std::shared_ptr<AnimationSequence> animSeq)
{
    switch (m_Direction)
    {
    case forward:
        SetParams(animSeq, SDL_FLIP_NONE);        
        break;
    case backward:
        SetParams(animSeq, SDL_FLIP_HORIZONTAL);        
        break;
    default:
        break;
    }
}

bool Character::CheckCollisions()
{
    bool isCollision {false};

    //  Calculate position where we are going to go

    m_Transform->Translate(m_RigidBody->getPosition());
    
    PositionRecalc();

    //  Check if there will still be collision in that position
    if (m_Collision->GetInstance()->CollisionWithMapX(&m_Transform, m_CollisionBox))
    {
        //  If yes - stop moving and position is getting corrected by CollisionWithMapX
        //  Recalc m_pos and collision box 
        m_RigidBody->StopX();
        PositionRecalc();
        isCollision = true;
    }

    //  Same for collsiion with Y                                      
    if (m_Collision->GetInstance()->CollisionWithMapY(&m_Transform, m_CollisionBox))
    {
        m_RigidBody->StopY();
        PositionRecalc();
        isCollision = true;
        
        if (m_Condition == Falling)
        {
            m_Condition = IsIdle;
            m_IsInAir = false;
        }
    } else if (m_Condition == IsIdle)
    {
        m_Condition = Falling;
        m_IsInAir = true;
    }

    return isCollision;
}