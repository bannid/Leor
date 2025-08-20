#include "game.h"
#include "easings.cpp"
#include "utils.cpp"
#include "game_interpolateds.cpp"
#include "game_random.cpp"
#include "arena.cpp"
#include "ui.cpp"
#include "game_lists_utils.cpp"

#define ASPECT_RATIO (16.0f / 9.0f)

//NOTE(Banni): Function to calculate the offset and dimensions of the renderable area.
inline void
CalculateGameArea(game_state* State,
                  window* Window)
{
    b32 WidthIsBigger = Window->Width > Window->Height;
    if(WidthIsBigger)
    {
        State->GameArea.Height = Window->Height;
        State->GameArea.Width = MIN(State->GameArea.Height * ASPECT_RATIO,
                                    (f32)Window->Width);
        State->GameArea.Height = State->GameArea.Width * (1.0f / ASPECT_RATIO);
    }
    else
    {
        State->GameArea.Width = Window->Width;
        State->GameArea.Height = MIN(State->GameArea.Width * (1.0f / ASPECT_RATIO),
                                     (f32)Window->Height);
        State->GameArea.Width = State->GameArea.Height * ASPECT_RATIO;
    }
    State->GameArea.XOffset = Window->Width * .5 - State->GameArea.Width * .5;
    State->GameArea.YOffset = Window->Height * .5 - State->GameArea.Height * .5;
}

inline void
MapMouseToWorld(game_state* State,
                input* Input)
{
    v2 MousePosition = Input->Mouse.Position;
    
    // NOTE(Banni): Mapping of mouse click
    f32 NormalX = (MousePosition.x - State->GameArea.XOffset) / State->GameArea.Width;
    f32 NormalY = (MousePosition.y - State->GameArea.YOffset) / State->GameArea.Height;
    
    Input->Mouse.Mapped= {
        NormalX * State->WorldWidth,
        NormalY * State->WorldHeight
    };
}

game_v2_interpolated* GetInterpolated(game_state* State)
{
    game_v2_interpolated Item = {};
    return InsertItem(&State->Interpolateds, &Item);
}

inline void
InitializeGame(game_state* State, platform_api* PlatformApi, window* Window, input* Input)
{
    State->WorldHeight = 800.0f;
    // NOTE(Banni): Random number initialization
    State->Random.Seed = 82348;
    State->WorldWidth = ASPECT_RATIO * State->WorldHeight;
    State->Layer = PlatformApi->CreateLayer(State->WorldWidth, State->WorldHeight);
    CalculateGameArea(State, Window);
    State->Initialized = true;
}

DLL_API Game_Update_And_Render(GameUpdateAndRender)
{
    game_state* State = (game_state*)Memory;
    if(!State->Initialized)
    {
        InitializeGame(State, PlatformApi, Window, Input);
    }
    if(Window->Resized)
    {
        CalculateGameArea(State, Window);
    }
    if(State->GameReloaded)
    {
        
        State->GameReloaded = false;
    }
    
    MapMouseToWorld(State, Input);
    
    // NOTE(Banni): Attach the layer where we are going to be drawing the game
    PlatformApi->AttachLayer(&State->Layer);
    PlatformApi->ClearAttachedLayer(v4(.3,.3,.3,1.));
    
    // Note(Banni): Bind back the default framebuffer and draw the game layer onto it
    game_area* GameArea = &State->GameArea;
    PlatformApi->AttachLayer(NULL);
    PlatformApi->DrawTexturedRectangle(v3(
                                          GameArea->XOffset,
                                          GameArea->YOffset,
                                          0),
                                       v2(
                                          GameArea->Width,
                                          GameArea->Height),
                                       State->Layer.TextureID
                                       );
}