/* date = August 1st 2025 7:31 pm */

#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "types.h"
#include "debug.h"
#include "utils.h"
#include "common_layer.h"
#include "arena.h"
#include "game_random.h"
#include "ui.h"
#include "game_interpolateds.h"

#include "game_lists.h"

// NOTE(Banni): Area to draw the game on screen.
struct game_area
{
    u32                                                Width;
    u32                                                Height;
    u32                                                XOffset;
    u32                                                YOffset;
};

struct game_state
{
    layer                                               Layer;
    f32                                                 WorldWidth;
    f32                                                 WorldHeight;
    game_area                                           GameArea;
    b32                                                 Initialized;
    b32                                                 GameReloaded;
    memory_arena                                        Arena;
    game_random                                         Random;
    game_v2_interpolated_list                           Interpolateds;
    // NOTE(Banni): Game
    
    
};

#endif //GAME_H
