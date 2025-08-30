/* date = August 1st 2025 7:31 pm */

#ifndef GAME_H
#define GAME_H

#include "common_layer.h"

struct game_state
{
    b32                                                 Initialized;
    b32                                                 GameReloaded;
    memory_arena                                        Arena;
    
    u32                                                 CubeModel;
    u32                                                 HouseModel;
    u32                                                 SphereModel;
    // NOTE(Banni): Simulation
    leor_physics_world                                  World;
    entity*                                             Player;
};

#endif //GAME_H
