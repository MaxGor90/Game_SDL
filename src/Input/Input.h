#ifndef H_INPUT
#define H_INPUT


#include <map>
#include "SDL.h"
#include <memory>




class Input
{
private:
    Input();
    ~Input()
    {}
    
    void KeyUp();
    void KeyDown();
    void MouseKeyUp();
    void UpdateMBState(SDL_Event& event);

    

    const Uint8* m_KeyStates;
    std::map<Uint8, Uint8> m_MouseButtonsState{};

public:


    static Input& getInstance()
    {
        static Input InputInstance;
        return InputInstance;
    }    

    void Listen();
    bool isKeyDown(SDL_Scancode key);
    bool isMouseButtonDown(Uint8 key);
    bool isMouseButtonUp(Uint8 key);

};





#endif /* H_INPUT */
