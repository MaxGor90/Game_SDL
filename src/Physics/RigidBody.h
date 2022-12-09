#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector2D.h"
#include "Global.h"


#define UNI_MASS 1.0f

#define FORWARD 1
#define BACKWARD -1
#define DOWNWARD 1
#define UPWARD -1


class RigidBody
{
public:
    RigidBody() :
        m_Mass {UNI_MASS},
        m_Gravity {GRAVITY}
    {}

    // Setter for mass and gravity
    inline void setMass(float m) { m_Mass = m; }
    inline void setGravity(float grav) { m_Gravity = grav; }

    // Getters
    inline float getMass()              { return m_Mass;            }
    inline Vector2D getAcceleration()   { return m_Acceleration;    }
    inline Vector2D getVelocity()       { return m_Velocity;        }
    inline Vector2D getPosition()       { return m_Position;        }

    // Force
    inline void ApplyForce(Vector2D F) { m_Force = F; }
    inline void ApplyForceX(float Fx) { m_Force.m_X = Fx; }
    inline void ApplyForceY(float Fy) { m_Force.m_Y = Fy; }
    inline void UnsetForce() { m_Force = Vector2D(0.0f, 0.0f); }
    inline void UnsetForceX() { m_Force.m_X = 0.0f; }
    inline void UnsetForceY() { m_Force.m_Y = 0.0f; }
    
    // Friction
    inline void ApplyFriction(Vector2D Fr) { m_Friction = Fr; }
    inline void UnsetFriction() { m_Friction = Vector2D(0.0f, 0.0f); }

    //  Update
    void Update(float dt)
    {
        m_Acceleration.m_X = (m_Force.m_X + m_Friction.m_X) / m_Mass;
        m_Acceleration.m_Y = m_Gravity + ((m_Force.m_Y + m_Friction.m_Y) / m_Mass);
        m_Velocity.m_X = m_Acceleration.m_X * dt;                       //  Keeping constant speed across X-axis
        m_Velocity.m_Y += m_Acceleration.m_Y * dt;                      //  Free fall acceleration across Y-axis
        m_Position = m_Velocity * dt;
    }

    void StopX()
    {
        m_Acceleration.m_X = 0.0f;
        m_Velocity.m_X = 0.0f;
        m_Position.m_X = 0.0f;
    }

    void StopY()
    {
        m_Acceleration.m_Y = 0.0f;
        m_Velocity.m_Y = 0.0f;
        m_Position.m_Y = 0.0f;
    }

    void Stop()
    {
        StopX();
        StopY();
    }

private:

    float m_Mass;
    float m_Gravity;

    Vector2D m_Force;
    Vector2D m_Friction;
    Vector2D m_Position;
    Vector2D m_Velocity;
    Vector2D m_Acceleration;

};

#endif /* RIGIDBODY_H */
