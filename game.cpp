#include "game.h"
#include "utils.cpp"
#include "arena.cpp"
#include "lists_utils.cpp"

#define ASPECT_RATIO (16.0f / 9.0f)

inline void
InitializeGame(game_state* State, input* Input, scene* Scene)
{
    entity E;
    InsertItem(&Scene->Entites, &E);
    
    State->Initialized = true;
}

DLL_API Game_Update(GameUpdate)
{
    game_state* State = (game_state*)Memory;
    if(!State->Initialized)
    {
        InitializeGame(State, Input, Scene);
    }
    if(State->GameReloaded)
    {
        State->GameReloaded = false;
    }
}