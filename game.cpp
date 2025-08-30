#include "game.h"
#include "utils.cpp"
#include "arena.cpp"
#include "lists_utils.cpp"
#include "transform.cpp"
#include "physics/collision/collision.cpp"
#include "physics/physics.cpp"


#define PLAYER_START_POSITION v3(0,3.2,0)

inline void
InitializeEntities(game_state* State,
                   scene* Scene)
{
    ResetList(&Scene->Entites);
    entity Player;
    InitTransform(&Player.Transform);
    Player.Transform.Position = PLAYER_START_POSITION;
    Player.ModelIndex = State->SphereModel;
    Player.Transform.Scale = v3(.7,2,.2);
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
    
    InitTransform(&House.Transform);
    House.Transform.Scale = v3(5, 2.25, 5);
    House.Transform.Position = v3(10, 2.25, -10);
    House.ModelIndex = State->HouseModel;
    InsertItem(&Scene->Entites, &House);

    InitTransform(&House.Transform);
    House.Transform.Scale = v3(4, 2.1, 14);
    House.Transform.Position = v3(10, 2.15, -50);
    House.Transform.Rotation = glm::angleAxis(glm::radians(15.0f), glm::vec3(1,0,0));
    House.ModelIndex = State->HouseModel;
    InsertItem(&Scene->Entites, &House);
    
}

DLL_API Game_Update(GameUpdate)
{
    game_state* State = (game_state*)Memory;
    if(!State->Initialized)
    {
        State->CubeModel = Api->LoadLModel("../assetsProcessed/cubeUntextured.obj.lmodel");
        State->SphereModel = Api->LoadLModel("../assetsProcessed/sphere.obj.lmodel");
        State->HouseModel = Api->LoadLModel("../assetsProcessed/cube.obj.lmodel");
        
        // NOTE(Banni): Initialize the entities
        InitializeEntities(State, Scene);
        
        // NOTE(Banni): Initialize the static mesh and stuff
        InitList(&State->Arena, &State->World.CollisionMesh, 10000);
        State->World.Player.Position = PLAYER_START_POSITION;
        State->World.Player.Velocity = v3(0);
        Api->SetCollisionMesh(Scene->Entites, &State->World);
        
        
        State->Initialized = true;
    }
    if(State->GameReloaded)
    {
        // InitializeEntities(State, Scene);
        // State->World.Player.Position = PLAYER_START_POSITION;
        // Api->SetCollisionMesh(Scene->Entites, &State->World);
        State->GameReloaded = false;
    }
    
    State->Player->Transform.Position = State->World.Player.Position;
    
    glm::quat YawQ = glm::angleAxis(glm::radians(State->World.Player.YawDegrees),
                                    glm::vec3(0,1,0));
    State->Player->Transform.Rotation = YawQ;
    
    Scene->ThirdPersonCamera.Target = State->Player->Transform.Position;
    Scene->ThirdPersonCamera.Yaw = State->World.Player.YawDegrees;
    UpdateWorld(&State->World, Input, Input->dt);
}