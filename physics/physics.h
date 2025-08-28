/* date = August 27th 2025 3:56 pm */

#ifndef PHYSICS_H
#define PHYSICS_H

#include "physics/collision/primitives.h"
#include "physics/collision/collision.h"

struct leor_physics_player
{
    v3 Position;
    v3 Velocity;
    f32 YawDegrees;
};

struct leor_physics_world
{
    leor_primitive_triangle_list CollisionMesh;
    leor_physics_player Player;
    u32 GPUHandle;
};

#endif //PHYSICS_H
