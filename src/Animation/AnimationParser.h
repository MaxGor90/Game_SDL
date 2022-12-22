#ifndef H_ANIMATIONPARSER
#define H_ANIMATIONPARSER


#include "AnimationSequence.h"
#include "tinyxml2.h"




class AnimationParser
{   
public:
    static std::shared_ptr<AnimationSequence> ParseAnimationSequence(tinyxml2::XMLElement* el);
};


#endif /* H_ANIMATIONPARSER */
