#include "game.h"
#include "utils.cpp"
#include "arena.cpp"
#include "lists_utils.cpp"
#include "transform.cpp"

#define PLAYER_START_POSITION v3(0, 13.2, 0)

inline void
InitializeEntities(game_state *State,
                   engine_api *Api,
                   renderer_scene *Scene)
{
    ResetList(&Scene->Entites);
    
    renderer_entity Player = {};
    InitTransform(&Player.Transform);
    Player.EnityFlags = ENTITY_FLAG_RENDERABLE;
    Player.MaterialHandle = State->RedMaterial;
    
    Player.Transform.Position = PLAYER_START_POSITION;
    Player.ModelHandle = State->CubeModel;
    Player.Transform.Scale = v3(.3, 1, .1);
    State->Player = InsertItem(&Scene->Entites, &Player);
    
    renderer_entity Ground = {};
    Ground.EnityFlags = ENTITY_FLAG_RENDERABLE | ENTITY_FLAG_COLLIDEABLE;
    InitTransform(&Ground.Transform);
    Ground.ModelHandle = State->GroundModel;
    Ground.MaterialHandle = Api->LoadMaterial(State->TexturedShader, v4(.1, .5, .1, 1.), State->NoiseTexture);
    
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
    
    renderer_entity Box = {};
    Box.EnityFlags = ENTITY_FLAG_RENDERABLE | ENTITY_FLAG_COLLIDEABLE;
    InitTransform(&Box.Transform);
    Box.ModelHandle = State->CubeModel;
    
    Box.MaterialHandle = Api->LoadMaterial(State->TexturedShader, v4(.4, .2, .1, 1.), State->NoiseTexture);
    
    Box.Transform.Scale = v3(10, 1, 10);
    Box.Transform.Position = v3(-20, 1, -20);
    InsertItem(&Scene->Entites, &Box);
    
}

DLL_API Game_Update(GameUpdate)
{
    game_state *State = (game_state *)Memory;
    if (!State->Initialized)
    {
        // NOTE(Banni): Models
        State->CubeModel = Api->LoadLModel("../assetsProcessed/cubeUntextured.obj.lmodel");
        State->HouseModel = Api->LoadLModel("../assetsProcessed/cube.obj.lmodel");
        State->GroundModel = Api->LoadLModel("../assetsProcessed/plane.obj.lmodel");
        
        // NOTE(Banni): Shaders
        State->ColourShader = Api->LoadShader("../assets/shaders/colour.vs.c", "../assets/shaders/colour.fs.c");
        State->TexturedShader = Api->LoadShader("../assets/shaders/texture.vs.c", "../assets/shaders/texture.fs.c");
        
        // NOTE(Banni): Textures
        State->NoiseTexture = Api->LoadTexture("../assets/textures/noise.png", 4, false);
        
        // NOTE(Banni): Materials
        State->RedMaterial = Api->LoadMaterial(State->ColourShader, v4(.9, .1, .1, 1.), 0);
        
        // NOTE(Banni): Initialize the entities
        InitializeEntities(State, Api, Scene);
        State->World->Player.Position = PLAYER_START_POSITION;
        State->World->Player.Velocity = v3(0);
        Api->SetCollisionMesh(Scene->Entites, State->World);
        
        State->Initialized = true;
    }
    if (State->GameReloaded)
    {
        InitializeEntities(State, Api, Scene);
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