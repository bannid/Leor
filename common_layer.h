/* date = July 30th 2025 8:06 pm */

/** This file provides a layer b/w game side and the engine side. The includes below are required in the game side.
 * We also need to include this file in the engine side as we need some definitions from this file. Mainly the update
 * callback into the game DLL.
 * 
 * Including this file in engine side wouldn't conflict with the already included headers as the HEADER GUARD would stop the
 * double include.
 */

#ifndef COMMON_LAYER_H
#define COMMON_LAYER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "types.h"
#include "debug.h"
#include "utils.h"
#include "lists.h"
#include "physics/collision/primitives.h"
#include "physics/physics.h"
#include "transform.h"
#include "renderer/model.h"
#include "renderer/camera.h"
#include "renderer/scene.h"
#include "arena.h"
#include "engine_api.h"

#define DLL_API extern "C" __declspec(dllexport)

struct button_state
{
    b32                                              IsDown;
    b32                                              HeldDown;
    b32                                              IsReleased;
    b32                                              JustWentDown;
};

struct mouse_input
{
    v2                                               Position;
    button_state                                     Left;
    button_state                                     Right;
};

struct keyboard_input
{
    button_state                                     Up;
    button_state                                     Down;
    button_state                                     Left;
    button_state                                     Right;
};

struct input
{
    f32                                              dt;
    mouse_input                                      Mouse;
    keyboard_input                                   Keyboard;
};

#define Game_Update(Name) void Name(engine_api* Api,\
input* Input,\
renderer_scene* Scene,\
void* Memory)
Game_Update(GameUpdateStub){}

typedef Game_Update(game_update);

#endif //COMMON_LAYER_H
