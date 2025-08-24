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
    transform                                           Camera;
    
    entity*                                             Player;
};

#endif //GAME_H
