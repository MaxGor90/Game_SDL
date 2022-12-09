#ifndef INPUT_H
#define INPUT_H


#include <map>
#include "SDL.h"




class Input
{
private:
    Input();
    ~Input()
    {
        delete s_InputInstance;
    }
    static Input* s_InputInstance;
    void KeyUp();
    void KeyDown();
    void MouseKeyUp();
    void UpdateMBState(SDL_Event& event);

    

    const Uint8* m_KeyStates;
    std::map<Uint8, Uint8> m_MouseButtonsState{};

public:
    static Input* getInstance()
    {
        return s_InputInstance = (s_InputInstance == nullptr)? new Input() : s_InputInstance;
    }    

    void Listen();
    bool isKeyDown(SDL_Scancode key);
    bool isMouseButtonDown(Uint8 key);
    bool isMouseButtonUp(Uint8 key);

};





#endif /* INPUT_H */
