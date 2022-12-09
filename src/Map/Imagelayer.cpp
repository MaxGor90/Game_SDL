#include "Imagelayer.h"
#include "Engine.h"
#include "Global.h"

void ImageLayer::Render()
{
    int x {m_OffsetX}, y {m_OffsetY};

    do
    {
        if (x > 0)
            x -= m_Image.Width * static_cast<int>(m_RepeatX);

        TextureManager::getInstance()->DrawImage(m_Image.Name, x, y, m_Image.Width, m_Image.Height, m_Speed);
        x += m_Image.Width * static_cast<int>(m_RepeatX);

        while ( m_RepeatX && (x < Engine::getInstance()->getMap()->getMapLength()))
        {
            TextureManager::getInstance()->DrawImage(m_Image.Name, x, y, m_Image.Width, m_Image.Height, m_Speed);
            x += m_Image.Width;
        }

        x = m_OffsetX;
        y += m_Image.Height;
    }
    while ( m_RepeatY && (y < SCREEN_HEIGHT) );
}

void ImageLayer::Update()
{

}
