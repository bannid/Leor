/* date = August 28th 2025 2:33 pm */

#ifndef COLLISION_H
#define COLLISION_H

struct collision_packet
{
    glm::vec3 EllipsoidSpace;
    
    // NOTE(Banni): Params in World space
    glm::vec3 W_Position;
    glm::vec3 W_Velocity;
    
    // NOTE(Banni): Params in Ellipsoid space
    glm::vec3 E_Position;
    glm::vec3 E_Velocity;
    glm::vec3 E_NormalizedVelocity;
    
    // Collision information
    b32 FoundCollision;
    f32 NearestDistance;
    glm::vec3 IntersectionPoint;
    int32 CollisionRecursionDepth;
};

b32 SphereCollidingWithTriangle(collision_packet* CollisionPacket,
                                glm::vec3 P0, glm::vec3 P1, glm::vec3 P2,
                                glm::vec3 TriangleNormal);

b32 CheckPointIsInTriangle(const glm::vec3& Point,
                           const glm::vec3& P0,
                           const glm::vec3& P1,
                           const glm::vec3& P2);

#endif //COLLISION_H
