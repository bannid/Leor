#include "game.h"
#include "utils.cpp"
#include "arena.cpp"
#include "lists_utils.cpp"
#include "transform.cpp"

#define ASPECT_RATIO (16.0f / 9.0f)

#define PLAYER_START_POSITION v3(0,1,0)

inline void
InitializeEntities(game_state* State,
                   scene* Scene)
{
    ResetList(&Scene->Entites);
    entity Player;
    InitTransform(&Player.Transform);
    Player.Transform.Position = PLAYER_START_POSITION;
    Player.ModelIndex = State->CubeModel;
    Player.Transform.Scale = v3(.58 / 2., 2./2., .2 / 2);
    State->Player = InsertItem(&Scene->Entites, &Player);
    
    entity Ground;
    InitTransform(&Ground.Transform);
    Ground.Transform.Position = v3(0,-2,0);
    Ground.Transform.Scale = v3(100, 1, 100);
    Ground.ModelIndex = State->CubeModel;
    InsertItem(&Scene->Entites, &Ground);
    
    
    entity House;
    InitTransform(&House.Transform);
    House.Transform.Scale = v3(5, 2.25, 5);
    House.Transform.Position = v3(-10, 2.25, -10);
    House.ModelIndex = State->HouseModel;
    InsertItem(&Scene->Entites, &House);
    
    
}

DLL_API Game_Update(GameUpdate)
{
    game_state* State = (game_state*)Memory;
    if(!State->Initialized)
    {
        State->CubeModel = PlatformApi->LoadLModel("../assetsProcessed/cubeUntextured.obj.lmodel");
        State->HouseModel = PlatformApi->LoadLModel("../assetsProcessed/cube.obj.lmodel");
        InitTransform(&Scene->Camera.Transform);
        Scene->Camera.Transform.Position = glm::vec3(0,2, 10);
        Scene->Camera.Transform.Rotation = glm::quatLookAt(v3(0,0,-1), v3(0,1,0));
        
        InitializeEntities(State, Scene);
        
        State->Initialized = true;
    }
    if(State->GameReloaded)
    {
        InitializeEntities(State, Scene);
        Scene->Camera.Transform.Position = glm::vec3(0,5, 20);
        Scene->Camera.Transform.Rotation = glm::quatLookAt(v3(0,0,-1), v3(0,1,0));
        State->GameReloaded = false;
    }
    
    f32 SpeedPerSecond = 2.4;
    
    if(Input->Keyboard.Up.IsDown)
    {
        State->Player->Transform.Position.z -= SpeedPerSecond * Input->dt;
    }
    if(Input->Keyboard.Down.IsDown)
    {
        State->Player->Transform.Position.z += SpeedPerSecond * Input->dt;
    }
    if(Input->Keyboard.Right.IsDown)
    {
        State->Player->Transform.Position.x += SpeedPerSecond * Input->dt;
    }
    if(Input->Keyboard.Left.IsDown)
    {
        State->Player->Transform.Position.x -= SpeedPerSecond * Input->dt;
    }
    
}