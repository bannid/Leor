#include "assetProcessing/asset.h"

UsesScratchArena b32
LoadLModel(const char* FilePath,
           memory_arena* Arena,
           memory_arena ScratchArena,
           leor_model* Model)
{
    u8* FileBuffer = (u8*)GetMemory(&ScratchArena, MEGABYTE(5));
    int32 BytesRead = Win32ReadEntireFileV2(FilePath, FileBuffer, MEGABYTE(5));
    if(BytesRead)
    {
        asset_leor_model_header* Header = (asset_leor_model_header*)FileBuffer;
        InitList(Arena, &Model->Meshes, Header->NumberOfMeshes);
        
        asset_leor_mesh_header_info* MeshHeaders = (asset_leor_mesh_header_info*)
        (FileBuffer + sizeof(asset_leor_model_header));
        
        for(int32 i = 0; i < Header->NumberOfMeshes; i++)
        {
            asset_leor_mesh_header_info* MeshHeader = MeshHeaders + i;
            
            leor_mesh Mesh = {};
            InitList(Arena, &Mesh.Vertices, MeshHeader->NumberOfVertices);
            
            leor_vertex* Vertices = (leor_vertex*)
            (FileBuffer + Header->DataOffset + MeshHeader->Offset);
            
            for(int32 j = 0; j < MeshHeader->NumberOfVertices; j++)
            {
                leor_vertex Vertex = Vertices[j];
                InsertItem(&Mesh.Vertices, &Vertex);
            }
            if(*MeshHeader->DiffuseTexture != '\0')
            {
                Utils_CopyString(MeshHeader->DiffuseTexture, Mesh.DiffuseTexture);
            }
            // TODO(Banni): Load the normal texture as well
            InsertItem(&Model->Meshes, &Mesh);
        }
        return true;
    }
    return false;
}