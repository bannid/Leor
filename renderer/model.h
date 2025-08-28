/* date = August 21st 2025 0:35 pm */

#ifndef MODEL_H
#define MODEL_H

#define LEOR_MODEL_TEXTURE_MAX_LENGTH 32

struct leor_vertex
{
    v3 Position;
    v3 Normal;
    v2 TextureCoords;
};
Declare_List(leor_vertex);

// NOTE(Banni): Vertices are stored in order in memory
struct leor_mesh
{
    leor_vertex_list Vertices;
    b32 LoadedToGPU;
    u32 GPUId;
    char DiffuseTexture[LEOR_MODEL_TEXTURE_MAX_LENGTH];
    u32 DiffuseTextureID;
};
Declare_List(leor_mesh);

struct leor_model
{
    leor_mesh_list Meshes;
    b32 LoadedToGPU;
};
Declare_List(leor_model);

#endif //MODEL_H
