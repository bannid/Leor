#include <windows.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad.c"

#include "types.h"
#include "debug.h"
#include "utils.h"

#include "common_layer.h"

#include "arena.h"
#include "arena.cpp"

#include "win32/win32_memory.h"
#include "win32/win32_file.h"
#include "win32/win32_file.cpp"
#include "win32/win32_memory.h"
#include "win32/win32_memory.cpp"
#include "win32/win32_dll.h"
#include "win32/win32_dll.cpp"

#include "fonts/fonts.h"
#include "fonts/fonts.cpp"

#include "opengl/framebuffer.h"
#include "opengl/framebuffer.cpp"
#include "opengl/model.h"
#include "opengl/shader.h"
#include "opengl/shader.cpp"
#include "opengl/texture.h"
#include "opengl/texture.cpp"

#include "transform.h"
#include "transform.cpp"
#include "model.h"
#include "scene.h"

#include "renderer/renderer.h"
#include "renderer/renderer.cpp"

#include "lists_utils.cpp"

#include "model.cpp"
#include "opengl/model.cpp"

#include "game.h"
#define ASPECT_RATIO (16.0f / 9.0f)


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
    local_persist mouse_input LastFrameMouse;
    
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
    LastFrameMouse = Input->Mouse;
}

int CALLBACK WinMain(HINSTANCE instance,
					 HINSTANCE prevInstance,
					 LPSTR commandLine,
					 int showCode)
{
    renderer Renderer;
    b32 Running = InitializeRenderer(&Renderer, 800, 800, "Leor");
    ASSERT_DEBUG(Running);
    memory_arena MainMemoryArena = Win32GetMemoryArena(MEGABYTE(500));
    // NOTE(Banni): If we failed to get memory from the OS.
    if(MainMemoryArena.BasePointer == NULL) return -1;
    memory_arena ScratchArena = GetMemoryArena(&MainMemoryArena,
                                               MEGABYTE(10));
    win32_game_code GameCode = Win32LoadGameDLL(false);
    input Input;
    
    game_state* GameState = (game_state*)GetMemory(&MainMemoryArena,
                                                   sizeof(game_state));
    ZeroMemory(GameState, sizeof(game_state));
    GameState->Arena = GetMemoryArena(&MainMemoryArena, MEGABYTE(20));
    
    scene DefaultScene = {};
    InitList(&MainMemoryArena, &DefaultScene.Entites, 100);
    
    memory_arena LoadModelArena = GetMemoryArena(&MainMemoryArena,
                                                 MEGABYTE(10));
    leor_model Model = {};
    LoadLModel("..\\assetsProcessed\\Inn.fbx.lmodel",
               &LoadModelArena,
               ScratchArena,
               &Model
               );
    // TODO(Banni): Load the model to open gl
    LoadLModelToGPU(&Model);
    
    // TODO(Banni): How to free up the memory that was used by the model loading?
    
    transform Camera;
    InitTransform(&Camera);
    Camera.Rotation = glm::quatLookAt(v3(0,0,1), v3(0,1,0));
    
    glm::mat4 ProjectionMat = glm::perspective(glm::radians(50.0f), 1.0f,
                                               .1f,
                                               100.0f);
    gl_model Cube = LoadCubeToGPU();
    
    transform EntityTransform;
    InitTransform(&EntityTransform);
    EntityTransform.Position = v3(0,0,20);
    EntityTransform.Scale = v3(2);
    
    shader_program MainShader = LoadShaderFromFile("../shaders/main.vs.c",
                                                   "../shaders/main.fs.c",
                                                   ScratchArena);
    f32 DeltaTime = 1.0f / 75.0f;
    while(1)
    {
        FILETIME NewDllWriteTime = Win32GetLastWriteTime(GAME_DLL_NAME);
        if(CompareFileTime(&NewDllWriteTime, &GameCode.LastWriteTime) != 0)
        {
            Win32UnloadGameDLL(&GameCode);
            GameCode = Win32LoadGameDLL(true);
            GameState->GameReloaded = true;
        }
        
        // TODO(Banni): Get the delta time
        //Input.dt = TargetSecondsPerFrame;
        GlfwProcessInput(Renderer.Window, &Input);
        
#if 0        
        GameCode.GameUpdate(&Input,
                            &DefaultScene,
                            (void*)GameState);
        // TODO(Banni): Render the default scene
        DrawScene(&Renderer, &DefaultScene);
#endif
        
        // NOTE(Banni): TEST CODE
        
        glm::quat Q = glm::angleAxis(glm::radians(10.0f) * DeltaTime,
                                     glm::vec3(1,0,0));
        EntityTransform.Rotation = Q * EntityTransform.Rotation;
        glClearColor(.0, .0, .0, .0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 ViewMat = glm::inverse(TransformToMat4(&Camera));
        glm::mat4 ModelMat = TransformToMat4(&EntityTransform);
        glUseProgram(MainShader.ID);
        glUniformMatrix4fv(glGetUniformLocation(MainShader.ID, "uProjection"),
                           1,
                           GL_FALSE,
                           glm::value_ptr(ProjectionMat));
        glUniformMatrix4fv(glGetUniformLocation(MainShader.ID, "uModel"),
                           1,
                           GL_FALSE,
                           glm::value_ptr(ModelMat));
        glUniformMatrix4fv(glGetUniformLocation(MainShader.ID, "uView"),
                           1,
                           GL_FALSE,
                           glm::value_ptr(ViewMat));     
        
        for(int32 i = 0; i < Model.Meshes.Length; i++)
        {
            leor_mesh* Mesh = GetItemPointer(&Model.Meshes, i);
            glBindVertexArray(Mesh->GPUId);
            glDrawArrays(GL_TRIANGLES, 0, Mesh->Vertices.Length);
        }
        // NOTE(Banni): End test code
        
        glfwSwapBuffers(Renderer.Window);
        glfwPollEvents();
        if(!RendererRunning(&Renderer))
        {
            break;
        }
    }
    return 0;
}