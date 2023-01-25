#include "AnimationParser.h"


/*   Method parses one animation sequence from XML with animation parameters: 
    name - name of the animation sequence
    row - row in the spritesheet
    frameCount - number of frames in animation
    startFrame - starting position in a row in the spritesheet
    frameTime - time per animation frame (1 / speed)
    Col<*> - collision box parameters inside the frame
    atkCol<*> - attack collision box parameters inside the frame (collision box for hit detection)
*/

std::shared_ptr<AnimationSequence> AnimationParser::ParseAnimationSequence(tinyxml2::XMLElement* el)
{
    std::string name;
    int row, frameCount, startFrame, frameTime;
    int colX, colY, colW, colH;
    animType type {animType::basic};
    int atkColX {0}, atkColY {0}, atkColW {0}, atkColH {0};
    int hitFrameStart {0}, hitFrames {0};
    std::shared_ptr<SDL_Rect> collisionBox {nullptr};
    std::shared_ptr<SDL_Rect> attackCollisionBox {nullptr};

    name = el->Attribute("name");
    row = el->IntAttribute("row");
    frameCount = el->IntAttribute("count");
    startFrame = el->IntAttribute("start");
    frameTime = el->IntAttribute("time");
    
    // Collisiom box parameters 
    colX = el->FindAttribute("x")? el->IntAttribute("x") : 0;
    colY = el->FindAttribute("y")? el->IntAttribute("y") : 0;
    colW = el->FindAttribute("width")? el->IntAttribute("width") : 0;
    colH = el->FindAttribute("height")? el->IntAttribute("height") : 0;
    if (!(colX==0 && colY==0 && colW==0 && colH==0))
        collisionBox = std::make_shared<SDL_Rect>(SDL_Rect{colX, colY, colW, colH});
    
    // Attack collsiion box parameters
    atkColX = el->FindAttribute("atkColX")? el->IntAttribute("atkColX") : 0;
    atkColY = el->FindAttribute("atkColY")? el->IntAttribute("atkColY") : 0;
    atkColW = el->FindAttribute("atkColW")? el->IntAttribute("atkColW") : 0;
    atkColH = el->FindAttribute("atkColH")? el->IntAttribute("atkColH") : 0;
    if (!(atkColX==0 && atkColY==0 && atkColW==0 && atkColH==0))
    {
        attackCollisionBox = std::make_shared<SDL_Rect>(SDL_Rect{atkColX, atkColY, atkColW, atkColH});
        type = animType::attack;
    }

    hitFrameStart = el->FindAttribute("hitFrameStart")? el->IntAttribute("hitFrameStart") : 0;
    hitFrames = el->FindAttribute("hitFrames")? el->IntAttribute("hitFrames") : 0;

    switch (type)
    {
    case animType::basic:
        return std::make_shared<AnimationSequence>(AnimationSequence(type, name, row, frameCount, startFrame, frameTime, collisionBox));
    case animType::attack:
    {
        // Making derived object and returning base class pointer
        return std::make_shared<AttackAnimationSequence>(AttackAnimationSequence( type, name, row, frameCount, startFrame, frameTime, 
                                                                            collisionBox, attackCollisionBox, hitFrameStart, hitFrames) );
    }
    default:
        return nullptr;
    }
}
