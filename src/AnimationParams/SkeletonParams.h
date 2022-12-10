#ifndef H_SKELETONPARAMS
#define H_SKELETONPARAMS

//  Collision Box for 128x82 Skeleton model. It's NOT centered so m_Direction must be taken into account
#define COLLISION_X             40
#define COLLISION_Y             14
#define COLLISION_WIDTH         33
#define COLLISION_HEIGHT        66

//  Collision boxes for attacks. Not centered, so m_Direction must be taken into consideration.
#define COLLISION_ATK1_X        27
#define COLLISION_ATK1_Y        0
#define COLLISION_ATK1_WIDTH    101
#define COLLISION_ATK1_HEIGHT   56

#define COLLISION_ATK2_X        4
#define COLLISION_ATK2_Y        16
#define COLLISION_ATK2_WIDTH    124
#define COLLISION_ATK2_HEIGHT   48

//  Animation parameters according to ../assets/Skeleton/Skeleton.png
//  Frame times are inversely proportional to animation speed
//  Idle
#define IDLE_FRAME_ROW          4
#define IDLE_FRAME_COUNT        5
#define IDLE_FRAME_START        0
#define IDLE_FRAME_TIME         80
//  Run
#define RUN_FRAME_ROW           3
#define RUN_FRAME_COUNT         12
#define RUN_FRAME_START         0
#define RUN_FRAME_TIME          80
//  Atk1
#define ATK1_FRAME_ROW          1
#define ATK1_FRAME_COUNT        7
#define ATK1_FRAME_START        0
#define ATK1_FRAME_TIME         60
//  ATK2
#define ATK2_FRAME_ROW          1
#define ATK2_FRAME_COUNT        6
#define ATK2_FRAME_START        7
#define ATK2_FRAME_TIME         60







#endif /* H_SKELETONPARAMS */
