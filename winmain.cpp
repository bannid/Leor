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

#include "opengl/framebuffer.cpp"
#include "opengl/model.h"
#include "opengl/model.cpp"
#include "opengl/shader.h"
#include "opengl/shader.cpp"
#include "opengl/texture.h"
#include "opengl/texture.cpp"

#include "lists.h"
#include "lists_utils.cpp"

#include "game.h"

#define ASPECT_RATIO (16.0f / 9.0f)


struct engine
{
    // NOTE(Banni): Shader used to draw rectangle
    shader_program                        RectangleShader;
    // NOTE(Banni): Shader used to render textures on a rectangle
    shader_program                        RectangleTexturedShader;
    shader_program                        FontShader;
    
    font_type                             Fonts;
    
    model                                 LeftAlignedQuad;
    model                                 CenterAlignedQuad;
    
    window                                Window;
    
    b32                                   Running;
};

global engine GlobalEngine;
inline void Win32IntToString(int32 Value, u8* Buffer, u32 BufferSize)
{
    snprintf((char*)Buffer, BufferSize, "%d",Value);
}

inline void 
UpdateShaderProjection(shader_program* Shader, mat4* Projection)
{
    glUseProgram(Shader->ID);
    glUniformMatrix4fv(Shader->ProjectionLocation,
                       1,
                       GL_FALSE,
                       (f32*)Projection);
}

void 
FrameBufferSizeCallback(GLFWwindow * Window, int32 Width, int32 Height)
{
    GlobalEngine.Window.Resized = true;
    GlobalEngine.Window.Height = Height;
    GlobalEngine.Window.Width = Width;
    
    mat4 Projection = glm::ortho(0.0f, (f32)Width,
                                 0.0f, (f32)Height,
                                 .0f, 100.0f);
    UpdateShaderProjection(&GlobalEngine.RectangleShader, &Projection);
    UpdateShaderProjection(&GlobalEngine.RectangleTexturedShader, &Projection);
    UpdateShaderProjection(&GlobalEngine.FontShader, &Projection);
    
    glViewport(0,0,Width,Height);
}

UsesScratchArena void
LoadGlobalEngine(int32 WindowWidth, int32 WindowHeight, memory_arena ScratchArena)
{
    GlobalEngine.Running = true;
    GlobalEngine.Window.Height = WindowHeight;
    GlobalEngine.Window.Width = WindowWidth;
    
    GlobalEngine.RectangleShader = LoadShaderFromFile("../shaders/rectangle.vs.c",
                                                      "../shaders/rectangle.fs.c",
                                                      ScratchArena);
    GlobalEngine.RectangleTexturedShader = LoadShaderFromFile("../shaders/rectangleTextured.vs.c",
                                                              "../shaders/rectangleTextured.fs.c",
                                                              ScratchArena);
    GlobalEngine.FontShader = LoadShaderFromFile("../shaders/font.vs.c",
                                                 "../shaders/font.fs.c",
                                                 ScratchArena);
    
    GlobalEngine.LeftAlignedQuad = LoadQuadModel(false);
    GlobalEngine.CenterAlignedQuad = LoadQuadModel(true);
    
    mat4 Projection = glm::ortho(0.0f, (f32)WindowWidth,
                                 0.0f, (f32)WindowHeight,
                                 .0f, 100.0f);
    
    UpdateShaderProjection(&GlobalEngine.RectangleShader, &Projection);
    UpdateShaderProjection(&GlobalEngine.RectangleTexturedShader, &Projection);
    UpdateShaderProjection(&GlobalEngine.FontShader, &Projection);
    // NOTE(Banni): Load fonts
    GlobalEngine.Fonts = FreeTypeLoadFontsFromFile("../assets/arial.ttf",
                                                   32.0f,
                                                   ScratchArena);
    
    ASSERT_DEBUG(GlobalEngine.RectangleShader.Valid);
    
}

inline void
DrawRectangle(v3 Position, v2 Scale, v4 Colour)
{
    mat4 Model = mat4(1.0f);
    Model = glm::translate(Model, Position);
    Model = glm::scale(Model, glm::vec3(Scale.x, Scale.y, 0.0f));
    glUseProgram(GlobalEngine.RectangleShader.ID);
    glUniform4fv(GlobalEngine.RectangleShader.ColourLocation, 1, (f32*)&Colour);
    glUniformMatrix4fv(GlobalEngine.RectangleShader.ModelLocation,
                       1,
                       GL_FALSE,
                       (f32*)&Model);
    glBindVertexArray(GlobalEngine.LeftAlignedQuad.VaoID);
    glDrawArrays(GL_TRIANGLES, 0, GlobalEngine.LeftAlignedQuad.VertCount);
}

inline void
DrawRectangle(v3 Position, v2 Scale, u32 TextureID)
{
    mat4 Model = mat4(1.0f);
    Model = glm::translate(Model, Position);
    Model = glm::scale(Model, glm::vec3(Scale.x, Scale.y, 0.0f));
    glUseProgram(GlobalEngine.RectangleTexturedShader.ID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glUniform1i(glGetUniformLocation(GlobalEngine.RectangleTexturedShader.ID,
                                     "uTexture"), 0);
    glUniformMatrix4fv(GlobalEngine.RectangleTexturedShader.ModelLocation,
                       1,
                       GL_FALSE,
                       (f32*)&Model);
    
    glBindVertexArray(GlobalEngine.LeftAlignedQuad.VaoID);
    glDrawArrays(GL_TRIANGLES, 0, GlobalEngine.LeftAlignedQuad.VertCount);
}


inline void
DrawRectangle(v3 Position, v2 Scale, u32 TextureID, shader_program* Shader)
{
    mat4 Model = mat4(1.0f);
    Model = glm::translate(Model, Position);
    Model = glm::scale(Model, glm::vec3(Scale.x, Scale.y, 0.0f));
    glUseProgram(Shader->ID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glUniform1i(glGetUniformLocation(Shader->ID,
                                     "uTexture"), 0);
    glUniformMatrix4fv(Shader->ModelLocation,
                       1,
                       GL_FALSE,
                       (f32*)&Model);
    glBindVertexArray(GlobalEngine.LeftAlignedQuad.VaoID);
    glDrawArrays(GL_TRIANGLES, 0, GlobalEngine.LeftAlignedQuad.VertCount);
}

inline void
AttachFramebuffer(framebuffer* FrameBuffer)
{
    f32 Width = FrameBuffer != NULL ? (f32)FrameBuffer->Width : GlobalEngine.Window.Width;
    f32 Height = FrameBuffer != NULL ? (f32)FrameBuffer->Height : GlobalEngine.Window.Height;
    mat4 Projection = glm::ortho(0.0f, Width,
                                 0.0f, Height,
                                 .0f, 100.0f);
    u32 ID = FrameBuffer != NULL ? FrameBuffer->ID : 0;
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glViewport(0,0, Width, Height);
    
    UpdateShaderProjection(&GlobalEngine.RectangleShader, &Projection);
    UpdateShaderProjection(&GlobalEngine.RectangleTexturedShader, &Projection);
    UpdateShaderProjection(&GlobalEngine.FontShader, &Projection);
}

inline void
ClearAttachedFramebuffer(v4 value)
{
    glClearColor(value.x, value.y, value.z, value.w);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(.0, .0, .0, .0);
    glClear(GL_DEPTH_BUFFER_BIT);
}

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
    y = GlobalEngine.Window.Height - y;
    Input->Mouse.Position = v2((f32)x, (f32)y);
    GlfwCheckState(&Input->Mouse.Left,
                   LastFrameMouse.Left,
                   glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT));
    GlfwCheckState(&Input->Mouse.Right,
                   LastFrameMouse.Right,
                   glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT));
    LastFrameMouse = Input->Mouse;
}

inline void
DrawText(const char* Text, v3 Position, f32 PixelSize)
{
    glDisable(GL_DEPTH_TEST);
    char C;
    
    while(C = *Text++)
    {
        font_character* Character = &GlobalEngine.Fonts.Characters[(int32)C];
        f32 xOffset = Character->Bearing.x;
        f32 yOffset = Character->Size.y - Character->Bearing.y;
        v2 scale = v2(PixelSize / GlobalEngine.Fonts.PixelHeight);
        v2 offset = { xOffset * scale.x, -yOffset * scale.y};
        
        v3 posToDraw = Position + v3(offset, .0);
        
        v2 scaleToDraw = 
        {
            Character->Size.x * scale.x,
            Character->Size.y * scale.y
        };
        
        f32 Advance = (Character->Advance >> 6) * scale.x;
        Position.x += Advance;
        DrawRectangle(posToDraw,
                      scaleToDraw,
                      Character->TextureID,
                      &GlobalEngine.FontShader);
        
    }
    glEnable(GL_DEPTH_TEST);
}

v2 MeasureText(const char* Text,
               f32 Size)
{
    f32 X = 0.0f;
    
    f32 ScaleX = Size / GlobalEngine.Fonts.PixelHeight;
    f32 ScaleY = Size / GlobalEngine.Fonts.PixelHeight;
    
    f32 H = 0.0f;
    while(*Text != 0)
    {
        char C = *Text;
        u32 Index = (u32)C;
        font_character Character = GlobalEngine.Fonts.Characters[Index];
        X += (Character.Advance >> 6) * ScaleX;
        H = FMAX(H, Character.Size.y * ScaleY);
        //Move to next character
        Text++;
    }
    return v2(X,H);
    
}

void ExitGame()
{
    GlobalEngine.Running = false;
}

int CALLBACK WinMain(HINSTANCE instance,
					 HINSTANCE prevInstance,
					 LPSTR commandLine,
					 int showCode)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    int32 Height = 800;
    int32 Width = Height * ASPECT_RATIO;
    
    const char* WindowName = "CommandRunner";
    
    GLFWwindow* Window = glfwCreateWindow(Width,
                                          Height,
                                          WindowName,
                                          NULL,
                                          NULL);
    ASSERT_DEBUG(Window);
    glfwMakeContextCurrent(Window);
    
    // NOTE(Banni): VSync ON
    glfwSwapInterval(1);
    
    // NOTE(Banni): Get the monitor refresh rate.
    f32 MonitorHertz = 60.0f;
    GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();
    if(PrimaryMonitor != NULL)
    {
        const GLFWvidmode* PrimaryMonitorVideoMode = glfwGetVideoMode(PrimaryMonitor);
        MonitorHertz = (f32)PrimaryMonitorVideoMode->refreshRate;
    }
    // NOTE(Banni): Set the target FPS
    f32 TargetSecondsPerFrame = 1.0f / MonitorHertz;
    
    glfwSetFramebufferSizeCallback(Window, FrameBufferSizeCallback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;
    
    // NOTE(Banni): GL Flags
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    memory_arena MainMemoryArena = Win32GetMemoryArena(MEGABYTE(50));
    // NOTE(Banni): If we failed to get memory from the OS.
    if(MainMemoryArena.BasePointer == NULL) return -1;
    memory_arena ScratchArena = GetMemoryArena(&MainMemoryArena,
                                               MEGABYTE(2));
    
    LoadGlobalEngine(Width, Height, ScratchArena);
    
    win32_game_code GameCode = Win32LoadGameDLL(false);
    
    platform_api PlatformApi;
    PlatformApi.DrawRectangle = &DrawRectangle;
    PlatformApi.LoadTexture = &LoadTexture;
    PlatformApi.CreateLayer = &CreateFrameBuffer;
    PlatformApi.ClearAttachedLayer = &ClearAttachedFramebuffer;
    PlatformApi.AttachLayer = &AttachFramebuffer;
    PlatformApi.DrawTexturedRectangle = &DrawRectangle;
    PlatformApi.DrawText = &DrawText;
    PlatformApi.IntToString = &Win32IntToString;
    PlatformApi.MeasureText = &MeasureText;
    PlatformApi.ExitGame = &ExitGame;
    input Input;
    
    game_state* GameState = (game_state*)GetMemory(&MainMemoryArena,
                                                   sizeof(game_state));
    ZeroMemory(GameState, sizeof(game_state));
    GameState->Arena = GetMemoryArena(&MainMemoryArena, MEGABYTE(20));
    while(!glfwWindowShouldClose(Window) && GlobalEngine.Running)
    {
        FILETIME NewDllWriteTime = Win32GetLastWriteTime(GAME_DLL_NAME);
        if(CompareFileTime(&NewDllWriteTime, &GameCode.LastWriteTime) != 0)
        {
            Win32UnloadGameDLL(&GameCode);
            GameCode = Win32LoadGameDLL(true);
            GameState->GameReloaded = true;
        }
        
        glClearColor(.0, .0, .0, .0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Input.dt = TargetSecondsPerFrame;
        GlfwProcessInput(Window, &Input);
        GameCode.GameUpdateAndRender(&PlatformApi,
                                     &Input,
                                     &GlobalEngine.Window,
                                     (void*)GameState);
        GlobalEngine.Window.Resized = false;
        
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    return 0;
}