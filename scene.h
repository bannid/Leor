/* date = August 20th 2025 11:17 am */

#ifndef SCENE_H
#define SCENE_H

struct camera
{
    transform Transform;
};

struct entity
{
    transform Transform;
    u32 ModelIndex;
};

struct entity_list
{
    entity* Items;
    u32 Size;
    u32 Length;
    b32 Initialized;
};

struct scene
{
    camera Camera;
    entity_list Entites;
};

#endif //SCENE_H
