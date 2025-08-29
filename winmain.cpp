#include <windows.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "types.h"
#include "debug.h"
#include "utils.h"

#include "transform.h"

#include "common_layer.h"

#include "arena.h"

#include "win32/win32.h"

#include "physics/physics.h"

#include "renderer/renderer.h"

// NOTE(Banni): Functions
#include "transform.cpp"
#include "win32/win32_file.cpp"
#include "win32/win32_memory.cpp"
#include "win32/win32_dll.cpp"
#include "arena.cpp"
#include "lists_utils.cpp"
#include "lists_utils_internal.cpp"
#include "glad.c"

#include "utils.cpp"

#include "renderer/opengl/fonts/fonts.cpp"
#include "renderer/opengl/framebuffer.cpp"
#include "renderer/opengl/shader.cpp"
#include "renderer/opengl/texture.cpp"
#include "renderer/opengl/model.cpp"
#include "renderer/model.cpp"
#include "renderer/camera.cpp"

#include "renderer/renderer.cpp"
#include "physics/collision/collision.cpp"

#include "engine_api.h"
#include "game.h"


#define WIDTH 1920
#define HEIGHT 1080
#define ASPECT_RATIO (f32)WIDTH/(f32)HEIGHT

// NOTE(Banni): Globals
global memory_arena                                 GlobalScractchArena;
global memory_arena                                 GlobalModelsMemoryArena;
global leor_model_list                              GlobalModelsList;

void GlfwCheckState(button_state* ButtonState,
                    button_state LastState,
                    int32 KeyState)
{
    ButtonState->IsDown = KeyState == GLFW_PRESS;
    ButtonState->HeldDown = KeyState == GLFW_REPEAT;
    ButtonState->IsReleased = KeyState == GLFW_RELEASE && LastState.IsDown;
    ButtonState->JustWentDown = ButtonState->IsDown && !LastState.IsDown;
}

void GlfwProcessInput(GLFWwindow* Window, input* Input)
{
    local_persist mouse_input         LastFrameMouse;
    local_persist keyboard_input      LastFrameKeyboard;
    
    // NOTE(Banni): Mouse input
    real64 x;
    real64 y;
    glfwGetCursorPos(Window, &x, &y);
    // TODO(Banni): Get the window height from the renderer
    //y = GlobalEngine.Window.Height - y;
    Input->Mouse.Position = v2((f32)x, (f32)y);
    GlfwCheckState(&Input->Mouse.Left,
                   LastFrameMouse.Left,
                   glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT));
    GlfwCheckState(&Input->Mouse.Right,
                   LastFrameMouse.Right,
                   glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT));
    
    GlfwCheckState(&Input->Keyboard.Up,
                   LastFrameKeyboard.Up,
                   glfwGetKey(Window, GLFW_KEY_UP));
    GlfwCheckState(&Input->Keyboard.Down,
                   LastFrameKeyboard.Down,
                   glfwGetKey(Window, GLFW_KEY_DOWN));
    GlfwCheckState(&Input->Keyboard.Right,
                   LastFrameKeyboard.Right,
                   glfwGetKey(Window, GLFW_KEY_RIGHT));
    GlfwCheckState(&Input->Keyboard.Left,
                   LastFrameKeyboard.Left,
                   glfwGetKey(Window, GLFW_KEY_LEFT));
    
    LastFrameMouse = Input->Mouse;
    LastFrameKeyboard = Input->Keyboard;
}

// NOTE(Banni): Platform API implementation
Load_L_Model(LoadLModelAndUploadToGPU)
{
    leor_model Model = {};
    LoadLModel(Path,
               &GlobalModelsMemoryArena,
               GlobalScractchArena,
               &Model);
    LoadLModelToGPU(&Model);
    InsertItem(&GlobalModelsList, &Model);
    return GlobalModelsList.Length - 1;
}

// NOTE(Banni): Platform API implementation
// TODO(Banni): Right now we dont have any way of distinguising b/w static and moving entities.
Set_Collision_Mesh(SetCollisionMesh)
{
    ResetList(&World->CollisionMesh);
    for(int32 i = 0; i < EntityList.Length; i++)
    {
        // TODO(Banni): TEMP - Do not add the player to the collision mesh
        if (i == 0) continue;
        entity Entity = *GetItemPointer(&EntityList, i);
        
        // NOTE(Banni): Make the collsion mesh little bit bigger then the actual models
        Entity.Transform.Scale *= 1.001;
        mat4 EntityTransform = TransformToMat4(&Entity.Transform);
        leor_model* Model = GetItemPointer(&GlobalModelsList, Entity.ModelIndex);
        for(int32 j = 0; j < Model->Meshes.Length; j++)
        {
            leor_mesh* Mesh = GetItemPointer(&Model->Meshes, j);
            for(int32 k = 0; k < Mesh->Vertices.Length; k+=3)
            {
                leor_primitive_triangle Triangle;
                v4 PositionOne = v4(GetItemPointer(&Mesh->Vertices, k)->Position, 1.0f);
                v4 PositionTwo =  v4(GetItemPointer(&Mesh->Vertices, k + 1)->Position, 1.0f);
                v4 PositionThree = v4(GetItemPointer(&Mesh->Vertices, k + 2)->Position, 1.0f);
                
                // NOTE(Banni): Bring the vertices to world space
                PositionOne = EntityTransform * PositionOne;
                PositionTwo = EntityTransform * PositionTwo;
                PositionThree = EntityTransform * PositionThree;
                
                Triangle.V1 = v3(PositionOne.x, PositionOne.y, PositionOne.z);
                Triangle.V2 = v3(PositionTwo.x, PositionTwo.y, PositionTwo.z);
                Triangle.V3 = v3(PositionThree.x, PositionThree.y, PositionThree.z);
                InsertItem(&World->CollisionMesh, &Triangle);
            }
        }
    }
    World->GPUHandle = LoadCollisionMeshToGPU(World->CollisionMesh).VaoID;
}

inline void
InitiateGlobals(memory_arena* Arena)
{
    GlobalModelsList = {};
    InitList(Arena, &GlobalModelsList, 100);
}

int CALLBACK WinMain(HINSTANCE instance,
					 HINSTANCE prevInstance,
					 LPSTR commandLine,
					 int showCode)
{
    renderer Renderer;
    b32 Running = InitializeRenderer(&Renderer, WIDTH, HEIGHT, "Leor");
    ASSERT_DEBUG(Running);
    memory_arena MainMemoryArena = Win32GetMemoryArena(MEGABYTE(500));
    // NOTE(Banni): If we failed to get memory from the OS.
    if(MainMemoryArena.BasePointer == NULL) return -1;
    GlobalScractchArena = GetMemoryArena(&MainMemoryArena,
                                         MEGABYTE(10));
    win32_game_code GameCode = Win32LoadGameDLL(false);
    input Input;
    
    game_state* GameState = (game_state*)GetMemory(&MainMemoryArena,
                                                   sizeof(game_state));
    ZeroMemory(GameState, sizeof(game_state));
    GameState->Arena = GetMemoryArena(&MainMemoryArena, MEGABYTE(20));
    
    // NOTE(Banni): Initiate globals
    InitiateGlobals(&MainMemoryArena);
    
    // NOTE(Banni): Initiate the default scene
    scene DefaultScene = {};
    InitializeThirdPersonCamera(&DefaultScene.ThirdPersonCamera, 20.0f);
    InitList(&MainMemoryArena, &DefaultScene.Entites, 100);
    
    
    GlobalModelsMemoryArena = GetMemoryArena(&MainMemoryArena,
                                             MEGABYTE(10));
    
    glm::mat4 ProjectionMat = glm::perspective(glm::radians(50.0f), 
                                               ASPECT_RATIO,
                                               .1f,
                                               500.0f);
    shader_program MainShader = LoadShaderFromFile("../shaders/main.vs.c",
                                                   "../shaders/main.fs.c",
                                                   GlobalScractchArena);
    shader_program CollisionMeshShader = LoadShaderFromFile("../shaders/collision_mesh.vs.c",
                                                            "../shaders/collision_mesh.fs.c",
                                                            GlobalScractchArena);
    
    engine_api Api;
    Api.LoadLModel = &LoadLModelAndUploadToGPU;
    Api.SetCollisionMesh = &SetCollisionMesh;
    
    f32 DeltaTime = 1.0f / 75.0f;
    f32 CurrentTime = glfwGetTime();
    f32 LastTime = CurrentTime;
    
    while(1)
    {
        FILETIME NewDllWriteTime = Win32GetLastWriteTime(GAME_DLL_NAME);
        if(CompareFileTime(&NewDllWriteTime, &GameCode.LastWriteTime) != 0)
        {
            Win32UnloadGameDLL(&GameCode);
            GameCode = Win32LoadGameDLL(true);
            // TODO(Banni): Temp code. Reload the shader.
            MainShader = LoadShaderFromFile("../shaders/main.vs.c",
                                            "../shaders/main.fs.c",
                                            GlobalScractchArena);
            GameState->GameReloaded = true;
        }
        
        GlfwProcessInput(Renderer.Window, &Input);
        CurrentTime = glfwGetTime();
        Input.dt = CurrentTime - LastTime;
        LastTime = CurrentTime;
        
        GameCode.GameUpdate(&Api,
                            &Input,
                            &DefaultScene,
                            (void*)GameState);
        
        // NOTE(Banni): Draw the actual scene
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        DrawScene(&Renderer,
                  &DefaultScene,
                  &MainShader,
                  &ProjectionMat,
                  &GlobalModelsList);
        
        
        // NOTE(Banni): Draw the collision mesh
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glm::mat4 CollisionViewMat = GetViewMatrix(&DefaultScene.ThirdPersonCamera);
        DrawCollisionMesh(&Renderer,
                          &CollisionMeshShader,
                          &GameState->World,
                          &CollisionViewMat,
                          &ProjectionMat
                          );
        
        glfwSwapBuffers(Renderer.Window);
        glfwPollEvents();
        if(!RendererRunning(&Renderer))
        {
            break;
        }
    }
    return 0;
}