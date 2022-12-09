#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <iostream>
#include <string>
#include "Vector2D.h"


//  Object positioning and movement class
class Transform
{
public:
    float m_X, m_Y;

    Transform(float x = 0, float y = 0):
        m_X {x}, m_Y {y}
    {}

    // void Log(std::string msg = "")
    // {
    //     std::cout << msg << " (X Y) = " << "(" <<  m_X << ' ' << m_Y << ");" << std::endl;
    // }

    inline void TranslateX(float x) { m_X += x; }   //  Moving across X-axis by x

    inline void TranslateY(float y) { m_Y += y; }   //  Moving across Y-axis by y

    inline void Translate(Vector2D vec)             //  Moving by Vector2D vec
    {
        m_X += vec.m_X;
        m_Y += vec.m_Y;
    }
};




#endif /* TRANSFORM_H */
