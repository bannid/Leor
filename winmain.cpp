#include <stdio.h>
#include <windows.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "types.h"
#include "utils.h"
#include "strings.h"
#include "lists.h"
#include "debug.h"
#include "debug_internal.h"
#include "transform.h"
#include "common_layer.h"
#include "debug_ui.h"
#include "arena.h"
#include "win32/win32.h"
#include "physics/physics.h"
#include "renderer/renderer.h"


// NOTE(Banni): Globals
global memory_arena                                    GlobalScractchArena;
global memory_arena                                    GlobalModelsMemoryArena;
global leor_model_list                                 GlobalModelsList;
global renderer                                        GlobalRenderer;

// NOTE(Banni): Global flags for Debugging i.e. If we should render collison mesh, frame times and stuff.
#if defined(DEBUG)
global timed_block_info_list                           GlobalFrameTimesDebugInfo;
global debug_state GlobalDebugState;
global debug_variable_list GlobalDebugVariableList;
#endif

// NOTE(Banni): Functions
#include "transform.cpp"
#include "win32/win32_file.cpp"
#include "win32/win32_memory.cpp"
#include "win32/win32_dll.cpp"
#include "arena.cpp"
#include "lists_utils.cpp"
#include "lists_utils_internal.cpp"

#if defined(DEBUG)
#include "lists_utils_debug.cpp"
#endif

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
#include "physics/physics.cpp"

#if defined(DEBUG)
#include "debugUI.cpp"
#endif

#include "engine_api.h"
#include "game.h"

#define WIDTH 1920
#define HEIGHT 1080
#define ASPECT_RATIO (f32) WIDTH / (f32)HEIGHT

#define MAX_TRIANGLES_IN_COLLISION_MESH              10000
#define MAX_MODELS                                   200
#define MAX_ENTITIES                                 200
#define MAX_DEBUG_VARIABLES                          200

#if defined(DEBUG)
void DebugPushVariableToGlobal(const char* Name, debug_variable_type Type, void* Pointer)
{
    debug_variable Variable;
    Variable.Name = Name;
    Variable.Type = Type;
    Variable.Pointer = Pointer;
    InsertItem(&GlobalDebugVariableList, &Variable);
}
#endif

void GlfwCheckState(button_state *ButtonState,
                    button_state LastState,
                    int32 KeyState)
{
    ButtonState->IsDown = KeyState == GLFW_PRESS;
    ButtonState->HeldDown = KeyState == GLFW_REPEAT;
    ButtonState->IsReleased = KeyState == GLFW_RELEASE && LastState.IsDown;
    ButtonState->JustWentDown = ButtonState->IsDown && !LastState.IsDown;
}

void GlfwProcessInput(GLFWwindow *Window, input *Input)
{
    local_persist mouse_input LastFrameMouse;
    local_persist keyboard_input LastFrameKeyboard;
    
    // NOTE(Banni): Mouse input
    real64 x;
    real64 y;
    glfwGetCursorPos(Window, &x, &y);
    // TODO(Banni): Get the window height from the renderer
    y = GlobalRenderer.Height - y;
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
    GlfwCheckState(&Input->Keyboard.Space,
                   LastFrameKeyboard.Space,
                   glfwGetKey(Window, GLFW_KEY_SPACE));
    
    LastFrameMouse = Input->Mouse;
    LastFrameKeyboard = Input->Keyboard;
}

inline void
InitiateGlobals(memory_arena *Arena)
{
    GlobalModelsList = {};
    InitList(Arena, &GlobalModelsList, MAX_MODELS);
    
    GlobalScractchArena = GetMemoryArena(Arena, MEGABYTE(10));
    GlobalModelsMemoryArena = GetMemoryArena(Arena, MEGABYTE(10));
}

#if defined(DEBUG)
inline void
InitiateGlobalDebugStuff(memory_arena *Arena)
{
    GlobalFrameTimesDebugInfo = {};
    InitList(Arena, &GlobalFrameTimesDebugInfo, MAX_DEBUG_VARIABLES);
    GlobalDebugState = {};
    GlobalDebugVariableList = {};
    InitList(Arena, &GlobalDebugVariableList, MAX_DEBUG_VARIABLES);
}

void PushDebugTimingInfo(timed_block_info Info)
{
    InsertItem(&GlobalFrameTimesDebugInfo, &Info);
}
#endif

API_LOAD_SHADER(LoadShaderGlobalRenderer)
{
    shader_program Shader = LoadShaderFromFile(VsFilePath, FsFilePath, GlobalScractchArena);
    InsertItem(&GlobalRenderer.Shaders, &Shader);
    return GlobalRenderer.Shaders.Length - 1;
}

API_LOAD_MATERIAL(LoadMaterialGlobalRenderer)
{
    renderer_material Material;
    Material.ShaderHandle = ShaderHandle;
    Material.Colour = Colour;
    Material.TextureHandle = TextureHandle;
    InsertItem(&GlobalRenderer.Materials, &Material);
    return GlobalRenderer.Materials.Length - 1;
}

API_LOAD_TEXTURE(LoadTextureGlobalRenderer)
{
    u32 TextureID = LoadTexture(FilePath, DesiredChannels, Flip);
    InsertItem(&GlobalRenderer.Textures, &TextureID);
    return GlobalRenderer.Textures.Length - 1;
}

// NOTE(Banni): Platform API implementation
API_LOAD_L_MODEL(LoadLModelAndUploadToGPU)
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
API_SET_COLLISION_MESH(SetCollisionMesh)
{
    ResetList(&World->CollisionMesh);
    for (int32 i = 0; i < EntityList.Length; i++)
    {
        renderer_entity Entity = *GetItemPointer(&EntityList, i);
        if (!(Entity.EnityFlags & ENTITY_FLAG_COLLIDEABLE))
            continue;
        mat4 EntityTransform = TransformToMat4(&Entity.Transform);
        leor_model *Model = GetItemPointer(&GlobalModelsList, Entity.ModelHandle);
        for (int32 j = 0; j < Model->Meshes.Length; j++)
        {
            leor_mesh *Mesh = GetItemPointer(&Model->Meshes, j);
            for (int32 k = 0; k < Mesh->Vertices.Length; k += 3)
            {
                leor_primitive_triangle Triangle;
                v4 PositionOne = v4(GetItemPointer(&Mesh->Vertices, k)->Position, 1.0f);
                v4 PositionTwo = v4(GetItemPointer(&Mesh->Vertices, k + 1)->Position, 1.0f);
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

void ImguiInit(GLFWwindow * window){
    
}
void ImguiNewFrame(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void ImguiRender(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void ImguiExit(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int CALLBACK WinMain(HINSTANCE instance,
                     HINSTANCE prevInstance,
                     LPSTR commandLine,
                     int showCode)
{
    
    // NOTE(Banni): Grab the memory from the OS
    memory_arena MainMemoryArena = Win32GetMemoryArena(MEGABYTE(500));
    
    // NOTE(Banni): If we failed to get memory from the OS.
    if (MainMemoryArena.BasePointer == NULL)
    {
        return -1;
    }
    
    // NOTE(Banni): Initiate globals
    InitiateGlobals(&MainMemoryArena);
#if defined(DEBUG)
    InitiateGlobalDebugStuff(&MainMemoryArena);
#endif
    
    // NOTE(Banni): Initialize the renderer
    
    b32 Running = InitializeRenderer(&GlobalRenderer,
                                     WIDTH, HEIGHT,
                                     "Leor",
                                     &MainMemoryArena,
                                     GlobalScractchArena);
    ASSERT_DEBUG(Running);
    
    // NOTE(Banni): Initialize the world
    leor_physics_world World = {};
    InitList(&MainMemoryArena, &World.CollisionMesh, MAX_TRIANGLES_IN_COLLISION_MESH);
    
    // NOTE(Banni): Game DLL loading and reloading
    win32_game_code GameCode = Win32LoadGameDLL(false);
    
    // NOTE(Banni): Keyboard/Mouse input
    input Input;
    
    // NOTE(Banni): Initialize the game memory from the OS side.
    game_state *GameState = (game_state *)GetMemory(&MainMemoryArena,
                                                    sizeof(game_state));
    ZeroMemory(GameState, sizeof(game_state));
    GameState->World = &World;
    GameState->Arena = GetMemoryArena(&MainMemoryArena, MEGABYTE(20));
    
    // NOTE(Banni): Initiate the default scene
    renderer_scene DefaultScene = {};
    InitializeThirdPersonCamera(&DefaultScene.ThirdPersonCamera, 20.0f);
    InitList(&MainMemoryArena, &DefaultScene.Entites, MAX_ENTITIES);
    
    glm::mat4 ProjectionMat = glm::perspective(glm::radians(50.0f),
                                               ASPECT_RATIO,
                                               .1f,
                                               500.0f);
    shader_program CollisionMeshShader = LoadShaderFromFile("../assets/shaders/collision_mesh.vs.c",
                                                            "../assets/shaders/collision_mesh.fs.c",
                                                            GlobalScractchArena);
    
    // NOTE(Banni): Set up the api to pass to the game DLL
    engine_api Api;
    Api.LoadLModel = &LoadLModelAndUploadToGPU;
    Api.SetCollisionMesh = &SetCollisionMesh;
    Api.LoadShader = &LoadShaderGlobalRenderer;
    Api.LoadMaterial = &LoadMaterialGlobalRenderer;
    Api.LoadTexture = &LoadTextureGlobalRenderer;
    
    f32 DeltaTime = 1.0f / 75.0f;
    f32 CurrentTime = glfwGetTime();
    f32 LastTime = CurrentTime;
    
#if defined(DEBUG)
    
    // NOTE(Banni): IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(GlobalRenderer.Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    debug_ui Ui;
    Ui.Flow = Debug_Ui_Flow_Vertical;
    Ui.Input = &Input;
#endif
    
    while (1)
    {
        TIMED_BLOCK_START(RenderLoop);
        FILETIME NewDllWriteTime = Win32GetLastWriteTime(GAME_DLL_NAME);
        if (CompareFileTime(&NewDllWriteTime, &GameCode.LastWriteTime) != 0)
        {
            Win32UnloadGameDLL(&GameCode);
            GameCode = Win32LoadGameDLL(true);
            GameState->GameReloaded = true;
        }
        
        TIMED_BLOCK_START(ProcessInput);
        GlfwProcessInput(GlobalRenderer.Window, &Input);
        TIMED_BLOCK_END(ProcessInput);
        
        DEBUG_PUSH_VARIABLE("Mouse position", Debug_Variable_Type_V2, &Input.Mouse.Position);
        CurrentTime = glfwGetTime();
        Input.dt = CurrentTime - LastTime;
        LastTime = CurrentTime;
        
        TIMED_BLOCK_START(GameDLL);
        GameCode.GameUpdate(&Api,
                            &Input,
                            &DefaultScene,
                            (void *)GameState);
        TIMED_BLOCK_END(GameDLL);
        
        UpdateWorld(&World, &Input);
        
        // NOTE(Banni): Draw the actual scene
        DrawScene(&GlobalRenderer,
                  &DefaultScene,
                  &ProjectionMat,
                  &GlobalModelsList);
        
#if defined(DEBUG)
        if (GlobalDebugState.DrawCollisionMesh)
        {
            // NOTE(Banni): Draw the collision mesh
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(2.0f);
            glm::mat4 CollisionViewMat = GetViewMatrix(&DefaultScene.ThirdPersonCamera);
            DrawModel(&GlobalRenderer,
                      &CollisionMeshShader,
                      World.GPUHandle,
                      &CollisionViewMat,
                      &ProjectionMat,
                      World.CollisionMesh.Length * 3);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        DrawDebugUI(&Ui, &Input);
        
#endif
        glfwSwapBuffers(GlobalRenderer.Window);
        glfwPollEvents();
        if (!RendererRunning(&GlobalRenderer))
        {
            break;
        }
        TIMED_BLOCK_END(RenderLoop);
    }
    
    return 0;
}