
#define GRAVITY glm::vec3(0,-25,0)

void
UpdateWorld(leor_physics_world* World, input* Input, f32 dt)
{
    World->dtAccumulator += Input->dt;
    while(World->dtAccumulator >= SIMULATION_FREQUENCY)
    {
        
        f32 RotationSpeedDeg = 180.0f;
        f32 MoveSpeed = 15.0f;
        f32 Speed = 1.0f;
        if(Input->Keyboard.Up.IsDown)
        {
            World->Player.Velocity += v3(0,0,-Speed);
        }
        if(Input->Keyboard.Down.IsDown)
        {
            World->Player.Velocity += v3(0,0,Speed);
        }
        if(Input->Keyboard.Right.IsDown)
        {
            World->Player.YawDegrees -= 50.0f * SIMULATION_FREQUENCY;
        }
        if(Input->Keyboard.Left.IsDown)
        {
            World->Player.YawDegrees += 50.0f * SIMULATION_FREQUENCY;
        }
        leor_physics_player* Player = &World->Player;
        glm::quat Q = glm::angleAxis(glm::radians(Player->YawDegrees),
                                    glm::vec3(0,1,0));
        v3 VelocityRotated = Q * Player->Velocity;
        
        
        v3 Velocity = VelocityRotated * SIMULATION_FREQUENCY;
        collision_packet Cp;
        Cp.EllipsoidSpace = v3(.7,2,.2);
        Cp.W_Position = Player->Position;
        Cp.W_Velocity = Velocity;
        Player->Position = CollideAndSlide(&Cp, World->CollisionMesh);

        Velocity = GRAVITY * SIMULATION_FREQUENCY;
        Cp.W_Position = Player->Position;
        Cp.W_Velocity = Velocity;
        Player->Position = CollideAndSlide(&Cp, World->CollisionMesh);
        Player->Velocity *= .98f;
        World->dtAccumulator -= SIMULATION_FREQUENCY;
    }
}