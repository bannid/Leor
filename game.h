/* date = August 1st 2025 7:31 pm */

#ifndef GAME_H
#define GAME_H

#include "common_layer.h"

#define METER(x)         (x)
#define CENTIMETER(x)    (x/100.0f)
#define KILOMETER(x)     (x*1000.0f)

struct game_state
{
    b32                                                 Initialized;
    b32                                                 GameReloaded;
    memory_arena                                        Arena;
    
    u32                                                 CubeModel;
    u32                                                 HouseModel;
    u32                                                 GroundModel;
    // NOTE(Banni): Simulation
    leor_physics_world*                                 World;
    renderer_entity*                                    Player;
};

#endif //GAME_H
