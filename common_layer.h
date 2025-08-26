/* date = July 30th 2025 8:06 pm */

#ifndef COMMON_LAYER_H
#define COMMON_LAYER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "types.h"
#include "debug.h"
#include "utils.h"
#include "transform.h"
#include "model.h"
#include "camera.h"
#include "scene.h"
#include "arena.h"
#include "platform_api.h"

#define DLL_API extern "C" __declspec(dllexport)

struct window
{
    u32                                              Width;
    u32                                              Height;
    b32                                              Resized;
};

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
    v2                                               Mapped;
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

#define Game_Update(Name) void Name(platform_api* PlatformApi,\
input* Input,\
scene* Scene,\
void* Memory)
Game_Update(GameUpdateStub){}

typedef Game_Update(game_update);

#endif //COMMON_LAYER_H
