
#include <iostream>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

#include "asset.h"

void WriteMeshFile(const char* FileName,
                   leor_vertex* Vertices,
                   u32 NumberOfVertices)
{
    FILE* FileToWrite = fopen(FileName, "wb");
    fwrite(&NumberOfVertices, sizeof(u32), 1, FileToWrite);
    fwrite(Vertices, sizeof(leor_vertex) * NumberOfVertices, 1, FileToWrite);
    fclose(FileToWrite);
}

asset_leor_mesh ProcessMesh(aiMesh* Mesh, const aiScene* Scene)
{
    leor_vertex* Vertices = (leor_vertex*)malloc(sizeof(leor_vertex) * Mesh->mNumVertices);   
    for(int32 i = 0; i < Mesh->mNumVertices; i++)
    {
        leor_vertex* Vertex = Vertices + i;
        Vertex->Position.x = Mesh->mVertices[i].x;
        Vertex->Position.y = Mesh->mVertices[i].y;
        Vertex->Position.z = Mesh->mVertices[i].z;
        if(Mesh->HasNormals())
        {
            Vertex->Normal.x = Mesh->mNormals[i].x;
            Vertex->Normal.y = Mesh->mNormals[i].y;
            Vertex->Normal.z = Mesh->mNormals[i].z;
        }
    }
    
    u32 TotalNumberOfVertices = 0;
    for(int32 i = 0; i < Mesh->mNumFaces; i++)
    {
        ASSERT_DEBUG(Mesh->mFaces[i].mNumIndices == 3);
        TotalNumberOfVertices += Mesh->mFaces[i].mNumIndices;
    }
    
    leor_vertex* Faces = (leor_vertex*)malloc(sizeof(leor_vertex) * TotalNumberOfVertices);
    
    for(int32 i = 0; i < Mesh->mNumFaces; i++)
    {
        aiFace Face = Mesh->mFaces[i];
        leor_vertex First = Vertices[Face.mIndices[0]];
        leor_vertex Second = Vertices[Face.mIndices[1]];
        leor_vertex Third = Vertices[Face.mIndices[2]];
        Faces[i * 3] = First;
        Faces[i * 3 + 1] = Second;
        Faces[i * 3 + 2] = Third;
    }
    
    free(Vertices);
    return
    {
        TotalNumberOfVertices,
        Faces
    };
}

void ProcessNode(aiNode* Node,
                 const aiScene* Scene,
                 std::vector<asset_leor_mesh>* Meshes)
{
    for(int32 i = 0; i < Node->mNumMeshes; i++)
    {
        aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
        Meshes->push_back(ProcessMesh(Mesh, Scene));
    }
    for(int32 i = 0; i < Node->mNumChildren; i++)
    {
        ProcessNode(Node->mChildren[i], Scene, Meshes);
    }
}

void
WriteModelFile(const char* FileName, const std::vector<asset_leor_mesh>* Meshes)
{
    FILE* FileToWrite = fopen(FileName, "wb");
    
    asset_leor_model_header Header;
    Header.NumberOfMeshes = Meshes->size();
    // NOTE(Banni): Where the actual data about meshes starts from
    Header.DataOffset = sizeof(asset_leor_model_header) 
        + sizeof(asset_leor_mesh_header_info) * Header.NumberOfMeshes;
    
    // NOTE(Banni): First write the header
    fwrite(&Header, sizeof(Header), 1, FileToWrite);
    
    // NOTE(Banni): Then write the info about each individual mesh
    // For example, how many vertices there are and the offset from 
    // the data offset
    for(int32 i = 0; i < Meshes->size(); i++)
    {
        asset_leor_mesh_header_info MeshHeader;
        MeshHeader.NumberOfVertices = (*Meshes)[i].NumberOfVertices;
        MeshHeader.Offset = i * sizeof(leor_vertex) * MeshHeader.NumberOfVertices;
        fwrite(&MeshHeader, sizeof(MeshHeader), 1, FileToWrite);
    }
    
    // NOTE(Banni): In last, write the actual meshes
    for(int32 i = 0; i < Meshes->size(); i++)
    {
        asset_leor_mesh Mesh = Meshes->at(i);
        fwrite(Mesh.Vertices,
               sizeof(leor_vertex) * Mesh.NumberOfVertices,
               1,
               FileToWrite);
    }
    
    fclose(FileToWrite);
    
}

u32 GetFileSize(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size;
    } else {
        perror("Error getting file size");
        return -1;
    }
}

// Note(Banni): This is how we read the processed file
void 
TestTheWrittenFile(const char* FileName)
{
    u32 FileWrittenSize = GetFileSize(FileName);
    u8* MemoryToReadWrittenFile = (u8*)malloc(FileWrittenSize);
    FILE* WrittenFile = fopen(FileName, "rb");
    fread(MemoryToReadWrittenFile, FileWrittenSize, 1, WrittenFile);
    fclose(WrittenFile);
    asset_leor_model_header* Header = (asset_leor_model_header*)MemoryToReadWrittenFile;
    std::cout << "Number of meshes: "
        << Header->NumberOfMeshes
        << std::endl;
    asset_leor_mesh_header_info* MeshHeaders = (asset_leor_mesh_header_info*)
        (MemoryToReadWrittenFile + sizeof(asset_leor_model_header));
    for(int32 i = 0; i < Header->NumberOfMeshes; i++)
    {
        asset_leor_mesh_header_info* MeshHeader = MeshHeaders + i;
        std::cout << "Mesh at position:"
            << i 
            << " has: "
            << MeshHeader->NumberOfVertices;
        leor_vertex* vertices = (leor_vertex*)(MemoryToReadWrittenFile + Header->DataOffset + MeshHeader->Offset);
        for(int32 j = 0; j < MeshHeader->NumberOfVertices; j++)
        {
            leor_vertex* vertex = vertices + j;

        }
    }

    free(MemoryToReadWrittenFile);
}

int main()
{
    const char* Path = "..\\assets\\House_4.fbx";
    Assimp::Importer Importer;
    const aiScene* Scene = Importer.ReadFile(Path,
                                             aiProcess_Triangulate |
                                             aiProcess_GenSmoothNormals |
                                             aiProcess_FlipUVs |
                                             aiProcess_CalcTangentSpace);
    if(!Scene ||
       Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
       !Scene->mRootNode)
    {
        std::cout 
            << "ERROR::ASSIMP:: " 
            << Importer.GetErrorString() 
            << std::endl;
        return 1;
    }
    std::vector<asset_leor_mesh> Meshes;
    ProcessNode(Scene->mRootNode, Scene, &Meshes);
    const char* FileToWriteName = "..\\assetsProcessed\\model.lmodel";
    WriteModelFile(FileToWriteName, &Meshes);
    // NOTE(Banni): Just for test
    TestTheWrittenFile(FileToWriteName);
    //End test
    
    std::cout << "Finished"
        << std::endl;
    return 0;
}