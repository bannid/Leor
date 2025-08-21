/* date = July 30th 2025 8:06 pm */

#ifndef COMMON_LAYER_H
#define COMMON_LAYER_H

#include "transform.h"
#include "scene.h"

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

struct input
{
    f32                                              dt;
    mouse_input                                      Mouse;
};

#define Game_Update(Name) void Name(input* Input,\
scene* Scene,\
void* Memory)
Game_Update(GameUpdateStub){}

typedef Game_Update(game_update);

#endif //COMMON_LAYER_H
