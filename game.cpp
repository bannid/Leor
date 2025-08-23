#include "game.h"
#include "utils.cpp"
#include "arena.cpp"
#include "lists_utils.cpp"
#include "transform.cpp"

#define ASPECT_RATIO (16.0f / 9.0f)

#define START_POSITION v3(4,1,-30)

DLL_API Game_Update(GameUpdate)
{
    game_state* State = (game_state*)Memory;
    if(!State->Initialized)
    {
        PlatformApi->LoadLModel("../assetsProcessed/inn.fbx.lmodel",
                                &State->Model,
                                &State->Arena);
        InitTransform(&Scene->Camera.Transform);
        Scene->Camera.Transform.Rotation = glm::quatLookAt(v3(0,0,-1), v3(0,1,0));
        
        entity Entity;
        InitTransform(&Entity.Transform);
        Entity.Transform.Position = START_POSITION ;
        Entity.Model = &State->Model;
        InsertItem(&Scene->Entites, &Entity);
        
        State->Initialized = true;
    }
    if(State->GameReloaded)
    {
        ResetList(&Scene->Entites);
        entity Entity;
        InitTransform(&Entity.Transform);
        Entity.Transform.Position = START_POSITION;
        Entity.Model = &State->Model;
        glm::quat Z = glm::angleAxis(glm::radians(90.0f),
                                     v3(0,0,1));
        glm::quat X = glm::angleAxis(glm::radians(-90.0f),
                                     v3(1,0,0));
        Entity.Transform.Rotation = Z * Entity.Transform.Rotation;
        Entity.Transform.Rotation = X * Entity.Transform.Rotation;
        
        Entity.Transform.Scale = v3(2);
        InsertItem(&Scene->Entites, &Entity);
        InsertItem(&Scene->Entites, &Entity);
        State->GameReloaded = false;
    }
    
    entity* Entity = GetItemPointer(&Scene->Entites, 0);
    glm::quat Y = glm::angleAxis(glm::radians(1.0f), v3(0,1,0));
    Entity->Transform.Rotation = Y * Entity->Transform.Rotation;
    
}