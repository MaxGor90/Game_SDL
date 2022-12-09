#ifndef H_KNIGHT_PARAMS
#define H_KNIGHT_PARAMS


//  Collision Box for 200x106 Knight model. It's centered so m_Direction doesn't affect it
#define COLLISION_X             77
#define COLLISION_Y             28
#define COLLISION_WIDTH         46
#define COLLISION_HEIGHT        78
#define COLLISION_ROLL_Y        54
#define COLLISION_ROLL_HEIGHT   52

//  Collision boxes for attacks. Not centered, so m_Direction must be taken into consideration.
#define COLLISION_ATK1_X        77
#define COLLISION_ATK1_Y        8
#define COLLISION_ATK1_WIDTH    113
#define COLLISION_ATK1_HEIGHT   78

#define COLLISION_ATK2_X        106
#define COLLISION_ATK2_Y        26
#define COLLISION_ATK2_WIDTH    84
#define COLLISION_ATK2_HEIGHT   54

#define COLLISION_ATK3_X        90
#define COLLISION_ATK3_Y        8
#define COLLISION_ATK3_WIDTH    106
#define COLLISION_ATK3_HEIGHT   98


//  Animation parameters according to ../assets/Knight/Knight.png
//  Frame times are inversely proportional to animation speed
//  Idle
#define IDLE_FRAME_ROW          3
#define IDLE_FRAME_COUNT        8
#define IDLE_FRAME_START        0
#define IDLE_FRAME_TIME         120
//  Run
#define RUN_FRAME_ROW           1
#define RUN_FRAME_COUNT         10
#define RUN_FRAME_START         0
#define RUN_FRAME_TIME          80
//  Roll
#define ROLL_FRAME_ROW          2
#define ROLL_FRAME_COUNT        9
#define ROLL_FRAME_START        0
#define ROLL_FRAME_TIME         60
//  Jump
#define JUMP_FRAME_ROW          5
#define JUMP_FRAME_COUNT        3
#define JUMP_FRAME_START        7
#define JUMP_FRAME_TIME         3
//  Fall
#define FALL_FRAME_ROW          4
#define FALL_FRAME_COUNT        4
#define FALL_FRAME_START        6
#define FALL_FRAME_TIME         60
//  Atk1
#define ATK1_FRAME_ROW          4
#define ATK1_FRAME_COUNT        6
#define ATK1_FRAME_START        0
#define ATK1_FRAME_TIME         60
//  ATK2
#define ATK2_FRAME_ROW          5
#define ATK2_FRAME_COUNT        6
#define ATK2_FRAME_START        0
#define ATK2_FRAME_TIME         60
//  ATK3
#define ATK3_FRAME_ROW          6
#define ATK3_FRAME_COUNT        8
#define ATK3_FRAME_START        0
#define ATK3_FRAME_TIME         50
//  Block
#define BLOCK_FRAME_ROW         9
#define BLOCK_FRAME_COUNT       8
#define BLOCK_FRAME_START       0
#define BLOCK_FRAME_TIME        80



#endif /* H_KNIGHT_PARAMS */
