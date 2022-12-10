#ifndef H_GAMEOBJECT
#define H_GAMEOBJECT

#include "IObject.h"
#include <string>
#include "SDL.h"
#include "Transform.h"
#include "Point.h"
#include "TextureManager.h"



struct ObjParams
{
    std::string TextureID;
    int Width, Height;
    float X, Y;
    SDL_RendererFlip Flip;
    
    ObjParams(  std::string id,
                float x, float y,
                SDL_RendererFlip flip = SDL_FLIP_NONE ) :
        TextureID {id},
        Width {TextureManager::getInstance()->GetTextureWidth(TextureID)}, 
        Height {TextureManager::getInstance()->GetTextureHeight(TextureID)},
        X {x}, Y {y},
        Flip {flip}
    {}        
};



class GameObject : public IObject
{
public:

    GameObject(std::shared_ptr<ObjParams> params):
        m_Width {params->Width},            //  Texture w
        m_Height {params->Height},          //  Texture h
        m_TextureID {params->TextureID},    //  Texture ID in map
        m_Flip {params->Flip}               //  Texture Flip
    {
        m_Transform = std::make_shared<Transform>(params->X, params->Y);  //  Object position

        float pos_x { m_Transform->m_X + params->Width / 2 };
        float pos_y { m_Transform->m_Y + params->Height / 2};
        m_Position = std::make_shared<Point>(pos_x, pos_y);
    }

    virtual ~GameObject()
    {}
    
    virtual void Draw() = 0;
    virtual void Update(float dt) = 0;
    virtual void Clean() = 0;

    inline std::shared_ptr<Point> GetPosition() { return m_Position; }

protected:

    std::shared_ptr<Transform> m_Transform {nullptr};
    int m_Width, m_Height;
    std::string m_TextureID;
    SDL_RendererFlip m_Flip;
    std::shared_ptr<Point> m_Position;

};

#endif /* H_GAMEOBJECT */
