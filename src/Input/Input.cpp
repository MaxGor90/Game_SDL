#include "Input.h"
#include "Engine.h"
#include "Global.h"


Input::Input()
{
    m_KeyStates = SDL_GetKeyboardState(nullptr);
    m_MouseButtonsState = {{MOUSE_LB, 0}, {MOUSE_MB, 0}, {MOUSE_RB, 0}, {MOUSE_B4, 0}, {MOUSE_B5, 0}};
}

void Input::Listen()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            Engine::getInstance().Quit();
            break;
        case SDL_KEYUP:
            KeyUp();
            break;
        case SDL_KEYDOWN:
            KeyDown();
            break;
        case SDL_MOUSEBUTTONDOWN:
            //  Fallthrough            
        case SDL_MOUSEBUTTONUP:
            UpdateMBState(event);
            break;
        default:
            break;
        }
    }
}

bool Input::isKeyDown(SDL_Scancode key)
{
    return (m_KeyStates[key] == 1);    
}

bool Input::isMouseButtonDown(Uint8 key)
{
    return m_MouseButtonsState.at(key); 
}

bool Input::isMouseButtonUp(Uint8 key)
{
        return (!m_MouseButtonsState.at(key)); 
}

void Input::KeyUp()
{
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::KeyDown()
{
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::UpdateMBState(SDL_Event& event)
{
    m_MouseButtonsState.at(event.button.button) = event.button.state;
}

