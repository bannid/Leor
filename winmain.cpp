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

#include "common_layer.h"

#include "arena.h"

#include "win32/win32_memory.h"
#include "win32/win32_file.h"
#include "win32/win32_memory.h"
#include "win32/win32_dll.h"

#include "fonts/fonts.h"

#include "opengl/framebuffer.h"
#include "opengl/model.h"
#include "opengl/shader.h"
#include "opengl/texture.h"

#include "transform.h"
#include "model.h"
#include "scene.h"

#include "renderer/renderer.h"

// NOTE(Banni): Functions
#include "glad.c"
#include "arena.cpp"
#include "win32/win32_file.cpp"
#include "win32/win32_memory.cpp"
#include "win32/win32_dll.cpp"
#include "fonts/fonts.cpp"
#include "opengl/framebuffer.cpp"
#include "opengl/shader.cpp"
#include "opengl/texture.cpp"
#include "transform.cpp"
#include "lists_utils.cpp"
#include "model.cpp"
#include "opengl/model.cpp"
#include "renderer/renderer.cpp"

#include "platform_api.h"
#include "game.h"


#define WIDTH 1920
#define HEIGHT 1080
#define ASPECT_RATIO (f32)WIDTH/(f32)HEIGHT


global memory_arena                    GlobalScractchArena;
global memory_arena                    GlobalModelsMemoryArena;
global leor_model_list                 GlobalModelsList;

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

u32 LoadLModelAndUploadToGPU(const char* Path)
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
    
    GlobalModelsList = {};
    InitList(&MainMemoryArena, &GlobalModelsList, 100);
    
    scene DefaultScene = {};
    InitList(&MainMemoryArena, &DefaultScene.Entites, 100);
    
    GlobalModelsMemoryArena = GetMemoryArena(&MainMemoryArena,
                                             MEGABYTE(10));
    
    glm::mat4 ProjectionMat = glm::perspective(glm::radians(50.0f), 
                                               ASPECT_RATIO,
                                               .1f,
                                               100.0f);
    shader_program MainShader = LoadShaderFromFile("../shaders/main.vs.c",
                                                   "../shaders/main.fs.c",
                                                   GlobalScractchArena);
    
    platform_api PlatformApi;
    PlatformApi.LoadLModel = &LoadLModelAndUploadToGPU;
    
    f32 DeltaTime = 1.0f / 75.0f;
    f32 CurrentTime = glfwGetTime();
    f32 LastTime = CurrentTime;
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
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
        
        GameCode.GameUpdate(&PlatformApi,
                            &Input,
                            &DefaultScene,
                            (void*)GameState);
        DrawScene(&Renderer,
                  &DefaultScene,
                  &MainShader,
                  &ProjectionMat,
                  &GlobalModelsList);
        
        glfwSwapBuffers(Renderer.Window);
        glfwPollEvents();
        if(!RendererRunning(&Renderer))
        {
            break;
        }
    }
    return 0;
}