#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <string>


class Vector2D
{
public:
    float m_X, m_Y;


    Vector2D(float x = 0.0f, float y = 0.0f):
        m_X {x}, m_Y {y}
    {}

    inline Vector2D operator+ (const Vector2D& vec) const 
    {
        return Vector2D(m_X + vec.m_X, m_Y + vec.m_Y);
    }

        inline Vector2D operator- (const Vector2D& vec)
    {
        return Vector2D(m_X - vec.m_X, m_Y - vec.m_Y);
    }

    inline Vector2D operator* (float scalar) const
    {
        return Vector2D(m_X * scalar, m_Y * scalar);
    }

    inline void operator+= (const Vector2D& vec)
    {
        m_X += vec.m_X;
        m_Y += vec.m_Y;
    }




    void Log(std::string msg = "")
    {
        std::cout << msg << " (X Y) = " << "(" <<  m_X << ' ' << m_Y << ");" << std::endl;
    }

};

#endif /* VECTOR2D_H */
