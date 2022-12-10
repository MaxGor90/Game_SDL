#ifndef H_CAMERA
#define H_CAMERA

#include "Vector2D.h"
#include "Point.h"
#include "SDL.h"
#include "memory"


class Camera
{
    static std::shared_ptr<Camera> s_CameraInstance;
    std::shared_ptr<Point> m_Target;
    Vector2D m_Position;
    SDL_Rect m_Viewbox;

public:
    Camera();
    ~Camera(); 

    inline static std::shared_ptr<Camera> getInstance()
    {
        return s_CameraInstance = (s_CameraInstance == nullptr)? std::make_shared<Camera>() : s_CameraInstance;
    }

    inline void SetTarget(std::shared_ptr<Point> P) { m_Target = P; }
    inline SDL_Rect GetViewbox() { return m_Viewbox; }
    inline Vector2D GetPosition() { return m_Position; }

    void Update(float dt);
  
};

#endif /* H_CAMERA */
