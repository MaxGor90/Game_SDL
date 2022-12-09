#include "Engine.h"
#include "Timer.h"

// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>


#ifdef __cplusplus
extern "C"
#endif


int main(int argc, char* argv[])
{   
    Engine::getInstance()->Init();

    while (Engine::getInstance()->isRunning())
    {
        Timer::getInstance()->Tick();
        Engine::getInstance()->Events();
        Engine::getInstance()->Update();
        Engine::getInstance()->Render();
    }

    Engine::getInstance()->Clean();
    



    // Avoid warnings for unused parameters
    argc = argc;
    argv = argv;


//    _CrtDumpMemoryLeaks();


    return 0;
}
