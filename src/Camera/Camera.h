#ifndef H_CAMERA
#define H_CAMERA

#include "Vector2D.h"
#include "Point.h"
#include "SDL.h"
#include "memory"


class Camera
{
    std::shared_ptr<Point> m_Target;
    Vector2D m_Position;
    SDL_Rect m_Viewbox;

    Camera();
    ~Camera(); 

public:

    inline static Camera& getInstance()
    {
        static Camera s_CameraInstance;
        return s_CameraInstance;
    }

    inline void SetTarget(std::shared_ptr<Point> P) { m_Target = P; }
    inline SDL_Rect GetViewbox() { return m_Viewbox; }
    inline Vector2D GetPosition() { return m_Position; }

    void Update();
  
};

#endif /* H_CAMERA */
