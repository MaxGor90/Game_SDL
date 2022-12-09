#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

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

    GameObject(ObjParams* params):
        m_Width {params->Width},            //  Texture w
        m_Height {params->Height},          //  Texture h
        m_TextureID {params->TextureID},    //  Texture ID in map
        m_Flip {params->Flip}               //  Texture Flip
    {
        m_Transform = new Transform(params->X, params->Y);  //  Object position

        float pos_x { m_Transform->m_X + params->Width / 2 };
        float pos_y { m_Transform->m_Y + params->Height / 2};
        m_Position = new Point(pos_x, pos_y);
    }

    virtual ~GameObject()
    {
        delete m_Transform;
        delete m_Position;
    }
    
    virtual void Draw() = 0;
    virtual void Update(float dt) = 0;
    virtual void Clean() = 0;

    inline Point* GetPosition() { return m_Position; }

protected:

    Transform* m_Transform {nullptr};
    int m_Width, m_Height;
    std::string m_TextureID;
    SDL_RendererFlip m_Flip;
    Point* m_Position;

};

#endif /* GAMEOBJECT_H */
