#include "game.h"
#include "utils.cpp"
#include "arena.cpp"
#include "lists_utils.cpp"
#include "transform.cpp"

#define PLAYER_START_POSITION v3(0, 13.2, 0)

inline void
InitializeEntities(game_state *State,
                   renderer_scene *Scene)
{
    ResetList(&Scene->Entites);
    renderer_entity Player = {};
    Player.EnityFlags = ENTITY_FLAG_RENDERABLE;
    InitTransform(&Player.Transform);
    Player.Transform.Position = PLAYER_START_POSITION;
    Player.ModelHandle = State->CubeModel;
    Player.Transform.Scale = v3(.3, 1, .1);
    State->Player = InsertItem(&Scene->Entites, &Player);
    
    renderer_entity Ground = {};
    Ground.EnityFlags = ENTITY_FLAG_RENDERABLE | ENTITY_FLAG_COLLIDEABLE;
    InitTransform(&Ground.Transform);
    Ground.ModelHandle = State->GroundModel;
    v3 GroundStart = v3(10, 0, 10);
    Ground.Transform.Scale = v3(METER(5), METER(5), METER(5));
    for(int32 x = 0; x < 10; x++)
    {
        for(int32 y = 0; y < 10; y++)
        {
            Ground.Transform.Position = GroundStart - v3(x * Ground.Transform.Scale.x * 2.,
                                                         METER(0),
                                                         y * Ground.Transform.Scale.z * 2.);
            InsertItem(&Scene->Entites, &Ground);
        }
    }
}

DLL_API Game_Update(GameUpdate)
{
    game_state *State = (game_state *)Memory;
    if (!State->Initialized)
    {
        State->CubeModel = Api->LoadLModel("../assetsProcessed/cubeUntextured.obj.lmodel");
        State->HouseModel = Api->LoadLModel("../assetsProcessed/cube.obj.lmodel");
        State->GroundModel = Api->LoadLModel("../assetsProcessed/plane.obj.lmodel");
        // NOTE(Banni): Initialize the entities
        InitializeEntities(State, Scene);
        State->World->Player.Position = PLAYER_START_POSITION;
        State->World->Player.Velocity = v3(0);
        Api->SetCollisionMesh(Scene->Entites, State->World);
        
        State->Initialized = true;
    }
    if (State->GameReloaded)
    {
        InitializeEntities(State, Scene);
        State->World->Player.Position = PLAYER_START_POSITION;
        Api->SetCollisionMesh(Scene->Entites, State->World);
        State->GameReloaded = false;
    }
    
    State->Player->Transform.Position = State->World->Player.Position;
    
    glm::quat YawQ = glm::angleAxis(glm::radians(State->World->Player.YawDegrees),
                                    glm::vec3(0, 1, 0));
    State->Player->Transform.Rotation = YawQ;
    
    Scene->ThirdPersonCamera.Target = State->Player->Transform.Position;
    Scene->ThirdPersonCamera.Yaw = State->World->Player.YawDegrees;
}