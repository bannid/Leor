/* date = August 20th 2025 11:17 am */

#ifndef SCENE_H
#define SCENE_H

#define ENTITY_FLAG_COLLIDEABLE 0x02
#define ENTITY_FLAG_RENDERABLE  0x04

struct renderer_entity
{
    transform Transform;
    u32 ModelHandle;
    u32 MaterialHandle;
    u32 EnityFlags;
};

DECLARE_LIST(renderer_entity);

struct renderer_scene
{
    third_person_camera ThirdPersonCamera;
    renderer_entity_list Entites;
};

#endif // SCENE_H
