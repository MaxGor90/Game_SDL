#ifndef IMAGELAYER_H
#define IMAGELAYER_H

#include "Layer.h"
#include "TextureManager.h"
#include <string>
#include <vector>



struct Image
{
    int Width, Height;
    std::string Name, Source;
};

class ImageLayer : public Layer
{
    Image m_Image;
    double m_Opacity;
    bool m_RepeatX, m_RepeatY;
    int m_OffsetX, m_OffsetY;

public:

    float m_Speed;

    ImageLayer(Image img, double opacity, bool repX, bool repY, int offX, int offY, float speed) :
        m_Image     {img},
        m_Opacity   {opacity},
        m_RepeatX   {repX},
        m_RepeatY   {repY},
        m_OffsetX   {offX},
        m_OffsetY   {offY},
        m_Speed     {speed}
    {
        TextureManager::getInstance().Load(img.Name, "../assets/Maps/" + img.Source, img.Width, img.Height, m_Opacity);
        LayerType = IMAGE;
    }

      void Render();
      void Update();
};

#endif // IMAGELAYER_H
