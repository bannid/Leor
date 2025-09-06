/* date = August 2nd 2025 2:26 pm */

#ifndef ENGINE_API_H
#define ENGINE_API_H

// NOTE(Banni): Loads a processed model to the engine.
#define API_LOAD_L_MODEL(Name) u32 Name(const char* Path)
typedef API_LOAD_L_MODEL(load_l_model);

// NOTE(Banni): Sets the collision mesh from the Entites
#define API_SET_COLLISION_MESH(Name) void Name(renderer_entity_list EntityList,\
leor_physics_world* World)
typedef API_SET_COLLISION_MESH(set_collision_mesh);

// NOTE(Banni): Exits the game
#define API_EXIT_GAME(Name) void Name()
typedef API_EXIT_GAME(exit_game);

#define API_LOAD_SHADER(Name) u32 Name(const char* VsFilePath, const char* FsFilePath)
typedef API_LOAD_SHADER(load_shader);

#define API_LOAD_MATERIAL(Name) u32 Name(u32 ShaderHandle, v4 Colour, u32 TextureHandle)
typedef API_LOAD_MATERIAL(load_material);

#define API_LOAD_TEXTURE(Name) u32 Name(const char* FilePath, u32 DesiredChannels, b32 Flip)
typedef API_LOAD_TEXTURE(load_texture);

struct engine_api
{
    load_l_model                          *LoadLModel;
    set_collision_mesh                    *SetCollisionMesh;
    load_shader                           *LoadShader;
    load_material                         *LoadMaterial;
    load_texture                          *LoadTexture;
    exit_game                             *ExitGame;
};

#endif //ENGINE_API_H
