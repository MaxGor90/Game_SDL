#ifndef POINT_H
#define POINT_H
#include <iostream>


class Point
{
public:

    float X {0}, Y {0};
    
    Point(float x = 0, float y = 0):
        X {x}, Y {y}
    {}

    Point operator+ (const Point& P) const { return Point(X + P.X, Y + P.Y); }
    Point operator- (const Point& P) const { return Point(X - P.X, Y - P.Y); }
    
    void operator= (const Point& P)
    {
        X = P.X;
        Y = P.Y;
    }
    void operator* (float scalar) 
    {
        X *= scalar;
        Y *= scalar;
    }

    void operator+= (const Point& P) 
    {
        X += P.X;
        Y += P.Y;
    }

    void operator-= (const Point& P) 
    {
        X -= P.X;
        Y -= P.Y;
    }

    void Log(const std::string& msg = "")
    {
        std::cout << msg << "(X Y)" << X << ' ' << Y << ")" << std::endl;
    }
    
};

#endif /* POINT_H */
