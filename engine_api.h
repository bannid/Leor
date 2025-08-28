/* date = August 2nd 2025 2:26 pm */

#ifndef ENGINE_API_H
#define ENGINE_API_H

// NOTE(Banni): Loads a processed model to the engine.
#define Load_L_Model(Name) u32 Name(const char* Path)
typedef Load_L_Model(load_l_model);

// NOTE(Banni): Sets the collision mesh from the Entites
#define Set_Collision_Mesh(Name) void Name(entity_list EntityList,\
leor_physics_world* World)
typedef Set_Collision_Mesh(set_collision_mesh);

// NOTE(Banni): Exits the game
#define Exit_Game(Name) void Name()
typedef Exit_Game(exit_game);

struct engine_api
{
    load_l_model*                          LoadLModel;
    set_collision_mesh*                    SetCollisionMesh;
    exit_game*                             ExitGame;
};

#endif //ENGINE_API_H
