/* date = August 21st 2025 2:42 pm */

#ifndef ASSET_H
#define ASSET_H

#if !defined(LEOR)

#include "../debug.h"
#include "../utils.h"
#include "../types.h"
#include "../model.h"

#endif
// NOTE(Banni): This is how the mesh is stored in the 
// filesystem
struct asset_leor_mesh
{
    u32 NumberOfVertices;
    leor_vertex* Vertices;
};

struct asset_leor_mesh_header_info
{
    u32 NumberOfVertices;
    u32 Offset;
};

struct asset_leor_model_header
{
    u32 NumberOfMeshes;
    u32 DataOffset;
};

struct asset_leor_model
{
    asset_leor_model_header Header;
    asset_leor_mesh_header_info MeshesMetaData;
    u8* Data;
};

#endif //ASSET_H
