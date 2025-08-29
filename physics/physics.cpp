
#define GRAVITY glm::vec3(0,-15,0)

void
UpdateWorld(leor_physics_world* World, f32 dt)
{
    leor_physics_player* Player = &World->Player;
    glm::quat Q = glm::angleAxis(glm::radians(Player->YawDegrees),
                                 glm::vec3(0,1,0));
    v3 VelocityRotated = Q * Player->Velocity;
    
    
    v3 Velocity = VelocityRotated * dt;
    collision_packet Cp;
    Cp.EllipsoidSpace = v3(1.,3,1.);
    Cp.W_Position = Player->Position;
    Cp.W_Velocity = Velocity;
    Player->Position = CollideAndSlide(&Cp, World->CollisionMesh);

    Velocity = GRAVITY * dt;
    Cp.W_Position = Player->Position;
    Cp.W_Velocity = Velocity;
    Player->Position = CollideAndSlide(&Cp, World->CollisionMesh);



    //Player->Position += Velocity;
    Player->Velocity *= .99f;
}