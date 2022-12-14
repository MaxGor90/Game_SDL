#include "Camera.h"
#include "Engine.h"
#include "Global.h"

std::shared_ptr<Camera> Camera::s_CameraInstance = nullptr;


Camera::Camera() : 
    m_Viewbox { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }
{}

Camera::~Camera()
{}


void Camera::Update(float dt)
{
    if (m_Target != nullptr)
    {
        m_Viewbox.x = m_Target->X - SCREEN_WIDTH / 2;
        m_Viewbox.y = m_Target->Y - SCREEN_HEIGHT / 2;
    
        if (m_Viewbox.x < 0)
            m_Viewbox.x = 0;
        if (m_Viewbox.y < 0)
            m_Viewbox.y = 0;

        if (m_Viewbox.x > (Engine::getInstance()->getMap()->getMapLength() - m_Viewbox.w))
            m_Viewbox.x = Engine::getInstance()->getMap()->getMapLength() - m_Viewbox.w;
        if (m_Viewbox.y > (SCREEN_HEIGHT - m_Viewbox.h))
            m_Viewbox.y = SCREEN_HEIGHT - m_Viewbox.h;

        m_Position = Vector2D(m_Viewbox.x, m_Viewbox.y);
    }
}

