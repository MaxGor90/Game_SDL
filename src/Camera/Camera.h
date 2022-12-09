#ifndef H_CAMERA
#define H_CAMERA

#include "Vector2D.h"
#include "Point.h"
#include "SDL.h"


class Camera
{
    Camera();
    ~Camera(); 

    static Camera* s_CameraInstance;
    Point* m_Target;
    Vector2D m_Position;
    SDL_Rect m_Viewbox;

public:

    inline static Camera* getInstance()
    {
        return s_CameraInstance = (s_CameraInstance == nullptr)? new Camera() : s_CameraInstance;
    }

    inline void SetTarget(Point* P) { m_Target = P; }
    inline SDL_Rect GetViewbox() { return m_Viewbox; }
    inline Vector2D GetPosition() { return m_Position; }

    void Update(float dt);
  
};

#endif /* H_CAMERA */
