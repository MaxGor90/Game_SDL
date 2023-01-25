#ifndef GLOBAL_H
#define GLOBAL_H

//  Mouse buttons codes
#define MOUSE_LB 1
#define MOUSE_RB 3
#define MOUSE_MB 2
#define MOUSE_B4 4
#define MOUSE_B5 5

const int SCREEN_WIDTH {1640};
const int SCREEN_HEIGHT {768};

const int TARGET_FPS {60};
const float TARGET_DELTATIME {1.3f};

const int attackOverlapX {5}; //    Number of pixels attack hitbox must overlap with character collision box tt score a hit 
 

#define GRAVITY 0.6f * (60.0f / TARGET_FPS) * (60.0f / TARGET_FPS)






#endif /* GLOBAL_H */
