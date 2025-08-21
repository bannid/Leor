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
#include "model.h"

struct game_state
{
    b32                                                 Initialized;
    b32                                                 GameReloaded;
    memory_arena                                        Arena;
};

#endif //GAME_H
