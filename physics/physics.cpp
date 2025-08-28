
void
UpdateWorld(leor_physics_world* World, f32 dt)
{
    leor_physics_player* Player = &World->Player;
    glm::quat Q = glm::angleAxis(glm::radians(Player->YawDegrees),
                                 glm::vec3(0,1,0));
    v3 VelocityRotated = Q * Player->Velocity;
    
    // TODO(Banni): Implement and call collide and slide here.
    
    Player->Position += VelocityRotated * dt;
    Player->Velocity *= .99f;
}