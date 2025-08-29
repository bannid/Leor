glm::vec3 CollideAndSlide(  collision_packet* Cp,
                            leor_primitive_triangle_list Triangles
)
{
    Cp->E_Velocity = Cp->W_Velocity / Cp->EllipsoidSpace;
    Cp->E_Position = Cp->W_Position / Cp->EllipsoidSpace;

    Cp->CollisionRecursionDepth = 0;

    glm::vec3 FinalPosition = CollideWithWorld(Cp, Triangles);

    FinalPosition = FinalPosition * Cp->EllipsoidSpace;

    return FinalPosition;
}

glm::vec3 CollideWithWorld( collision_packet* Cp, 
                            leor_primitive_triangle_list Triangles)
{
    f32 VeryCloseDistance = .05f;

    if(Cp->CollisionRecursionDepth > 3)
    {
        return Cp->E_Position;
    }

    Cp->E_NormalizedVelocity = glm::normalize(Cp->E_Velocity);
    Cp->FoundCollision = false;
    Cp->NearestDistance = 0.0f;

    for(int32 Tricounter = 0; Tricounter < Triangles.Length; Tricounter++)
    {
        leor_primitive_triangle* Triangle = GetItemPointer(&Triangles, Tricounter);
        glm::vec3 P0 = Triangle->V1;
        glm::vec3 P1 = Triangle->V2;
        glm::vec3 P2 = Triangle->V3;
        P0 = P0 / Cp->EllipsoidSpace;
        P1 = P1 / Cp->EllipsoidSpace;
        P2 = P2 / Cp->EllipsoidSpace;

        glm::vec3 TriangleNormal = glm::normalize(glm::cross(P1-P0, P2-P0));
        SphereCollidingWithTriangle(Cp, P0, P1, P2, TriangleNormal);
    }
    if(!Cp->FoundCollision)
    {
        return Cp->E_Position + Cp->E_Velocity;
    }

    glm::vec3 DestinationPoint = Cp->E_Position + Cp->E_Velocity;
    glm::vec3 NewPosition = Cp->E_Position; // Just for initialization

    // NOTE(Banni): Move the sphere toward the intersection point until it
    // almost touches the triangle plane
    if(Cp->NearestDistance >= VeryCloseDistance)
    {
        glm::vec3 V = Cp->E_Velocity;
        V = glm::normalize(V);
        V = V * (Cp->NearestDistance - VeryCloseDistance);
        NewPosition = Cp->E_Position + V;
        V = glm::normalize(V);
        Cp->IntersectionPoint -= VeryCloseDistance * V;
    }

    glm::vec3 SlidePlaneOrigin = Cp->IntersectionPoint;
    glm::vec3 SlidePlaneNormal = glm::normalize(NewPosition - Cp->IntersectionPoint);

    f32 x = SlidePlaneOrigin.x;
    f32 y = SlidePlaneOrigin.y;
    f32 z = SlidePlaneOrigin.z;

    f32 A = SlidePlaneNormal.x;
    f32 B = SlidePlaneNormal.y;
    f32 C = SlidePlaneNormal.z;

    f32 D = -((A*x) + (B*y) + (C*z));
    f32 PlaneConstant = D;

    f32 SignedDistanceFromDestinationPointToSlidingPlane = glm::dot(DestinationPoint, SlidePlaneNormal) + PlaneConstant;

    glm::vec3 NewDestinationPoint = DestinationPoint - SignedDistanceFromDestinationPointToSlidingPlane * SlidePlaneNormal;

    glm::vec3 NewVelocityVector = NewDestinationPoint - Cp->IntersectionPoint;

    // NOTE(Banni): If the velocity vector is really small, then just return
    if(glm::length(NewVelocityVector) < VeryCloseDistance)
    {
        return NewPosition;
    }

    Cp->CollisionRecursionDepth++;
    Cp->E_Position = NewPosition;
    Cp->E_Velocity = NewVelocityVector;

    return CollideWithWorld(Cp, Triangles);
}

b32 SphereCollidingWithTriangle(collision_packet* Cp,
                                glm::vec3 P0, glm::vec3 P1, glm::vec3 P2,
                                glm::vec3 TriangleNormal)
{
    // NOTE(Banni): This function assumes that TriangleNormal, and all the vertices
    // have been brought into Ellipsoid space
    b32 Facing = glm::dot(TriangleNormal, Cp->E_Velocity);
    if(Facing <= 0)
    {
        glm::vec3 Velocity = Cp->E_Velocity;
        glm::vec3 Position = Cp->E_Position;
        
        f32 t0, t1;
        
        b32 SphereIsInPlane = false;
        
        // NOTE(Banni): Find the plane in which the triangle lies in
        f32 x = P0.x;
        f32 y = P0.y;
        f32 z = P0.z;
        
        f32 A = TriangleNormal.x;
        f32 B = TriangleNormal.y;
        f32 C = TriangleNormal.z;
        
        f32 D = -((A * x) + (B * y) + (C * z));
        
        f32 PlaneConstant = D;
        
        f32 SignedDistanceFromPositionToTriPlane = glm::dot(Position, TriangleNormal) + PlaneConstant;
        
        f32 PlaneNormalDotVelocity = glm::dot(TriangleNormal, Velocity);
        
        // NOTE(Banni): If the velocity is parallel to plane
        if(PlaneNormalDotVelocity == 0.0f)
        {
            // NOTE(Banni): We are in ellipsoid space, which means from our perspective
            // we are a 1 unit sphere
            if(fabs(SignedDistanceFromPositionToTriPlane) >= 1.0f)
            {
                // NOTE(Banni): If we are parallel to the triangle plane
                // and more than one unit away from the triangle plane,
                // no collision is possible.
                return false;
            }
            else
            {
                // NOTE(Banni): Otherwise set the SphereIsInPlane = true.
                SphereIsInPlane = true;
            }
        }
        else
        {
            // NOTE(Banni): If the velocity is not parallel to the triangle plane,
            // we know the velocity vector will at some point intersect with the triangle
            // plane.
            
            t0 = (1.0f - SignedDistanceFromPositionToTriPlane)  / PlaneNormalDotVelocity;
            t1 = (-1.0f - SignedDistanceFromPositionToTriPlane) / PlaneNormalDotVelocity;
            
            if (t0 > t1)
            {
                f32 Temp = t0;
                t0 = t1;
                t1 = Temp;
            }
            
            if(t0 > 1.0f || t1 < 0.0f)
            {
                // NOTE(Banni): The velocity vector segment doesn't intersect with the plane.
                // Only the infinite velocity ray intersects with the triangle plane.
                return false;
            }
            
            if(t0 < 0.0f) t0 = 0.0f;
            if(t1 > 1.0f) t1 = 1.0f;
        }
        
        // NOTE(Banni): If we have made this far, we know that sphere will intersect with the triangle plane.
        
        // NOTE(Banni): Sphere inside triangle test
        
        glm::vec3 CollisionPoint;
        b32 CollidingWithTriangle = false;
        f32 t = 1.0f;
        
        // NOTE(Banni): If sphere is not embedded in the triangle plane
        if(!SphereIsInPlane)
        {
            glm::vec3 PlaneIntersectionPoint = Position + t0 * Velocity - TriangleNormal;
            if(CheckPointIsInTriangle(PlaneIntersectionPoint, P0, P1, P2))
            {
                CollidingWithTriangle = true;
                t = t0;
                CollisionPoint = PlaneIntersectionPoint;
            }
        }
        
        // NOTE(Banni): If we are not colliding with the inside of the triangle
        // then we need to check all the vertices.
        if (CollidingWithTriangle == false)
        {
            f32 a,b,c;
            f32 VelocityLengthSquared = glm::length(Velocity);
            VelocityLengthSquared *= VelocityLengthSquared;
            a = VelocityLengthSquared;
            
            // NOTE(Banni): Temporary variable to hold the distance down the velocity vector where the 
            // sphere will collide with the vertex
            f32 NewT;
            
            ////////////////// Collision test with vertices of the triangle //////////////////////////////
            
            // NOTE(Banni): Collision test with P0
            b = 2.0f * glm::dot(Velocity, Position - P0);
            c = glm::length(P0-Position);
            c = (c*c) - 1.0f;
            if(GetLowestRoot(a,b,c,t,&NewT))
            {
                t = NewT;
                CollidingWithTriangle = true;
                CollisionPoint = P0;
            }
            
            // NOTE(Banni): Collision test with P1
            b = 2.0f * glm::dot(Velocity, Position - P1);
            c = glm::length(P1-Position);
            c = (c*c) - 1.0f;
            if(GetLowestRoot(a,b,c,t,&NewT))
            {
                t = NewT;
                CollidingWithTriangle = true;
                CollisionPoint = P1;
            }
            
            // NOTE(Banni): Collsion test with P2
            b = 2.0f * glm::dot(Velocity, Position - P2);
            c = glm::length(P2-Position);
            c = (c*c) - 1.0f;
            if(GetLowestRoot(a,b,c,t,&NewT))
            {
                t = NewT;
                CollidingWithTriangle = true;
                CollisionPoint = P2;
            }
            
            ///////////////////// Collision with edges of the triangle ////////////////////////////
            
            // NOTE(Banni): Even though there might have collision with the vertices, we still need 
            // to check if we have collided with the edges.
            
            // NOTE(Banni): Edge (p0,p1)
            glm::vec3 Edge = P1 - P0;
            glm::vec3 SpherePositionToVertex = P0 - Position;
            
            f32 EdgeLengthSquared = glm::length(Edge);
            EdgeLengthSquared *= EdgeLengthSquared;
            
            f32 EdgeDotVelocity = glm::dot(Edge, Velocity);
            f32 EdgeDotSpherePositionToVertex = glm::dot(Edge, SpherePositionToVertex);
            f32 SpherePositionToVertexLengthSquared = glm::length(SpherePositionToVertex);
            SpherePositionToVertexLengthSquared *= SpherePositionToVertexLengthSquared;
            
            a = EdgeLengthSquared * -VelocityLengthSquared + (EdgeDotVelocity * EdgeDotVelocity);
            b = EdgeLengthSquared * (2.0f * glm::dot(Velocity, SpherePositionToVertex)) - (2.0f * EdgeDotVelocity * EdgeDotSpherePositionToVertex);
            c = EdgeLengthSquared * (1.0f - SpherePositionToVertexLengthSquared) + (EdgeDotSpherePositionToVertex * EdgeDotSpherePositionToVertex);
            
            if(GetLowestRoot(a,b,c,t,&NewT))
            {
                // NOTE(Banni): The velocity vector does intersect with the inifinite edge ray
                // Now we check if the intersection point lies in the edge segment
                f32 f = (EdgeDotVelocity * NewT - EdgeDotSpherePositionToVertex) / EdgeLengthSquared;
                if(f >= 0.0f && f <= 1.0f)
                {
                    t = NewT;
                    CollidingWithTriangle = true;
                    CollisionPoint = P0 + f * Edge;
                }
            }
            
            // NOTE(Banni): Edge (p1, p2)
            Edge = P2 - P1;
            SpherePositionToVertex = P1 - Position;
            
            EdgeLengthSquared = glm::length(Edge);
            EdgeLengthSquared *= EdgeLengthSquared;
            
            EdgeDotVelocity = glm::dot(Edge, Velocity);
            EdgeDotSpherePositionToVertex = glm::dot(Edge, SpherePositionToVertex);
            SpherePositionToVertexLengthSquared = glm::length(SpherePositionToVertex);
            SpherePositionToVertexLengthSquared *= SpherePositionToVertexLengthSquared;
            
            a = EdgeLengthSquared * -VelocityLengthSquared + (EdgeDotVelocity * EdgeDotVelocity);
            b = EdgeLengthSquared * (2.0f * glm::dot(Velocity, SpherePositionToVertex)) - (2.0f * EdgeDotVelocity * EdgeDotSpherePositionToVertex);
            c = EdgeLengthSquared * (1.0f - SpherePositionToVertexLengthSquared) + (EdgeDotSpherePositionToVertex * EdgeDotSpherePositionToVertex);
            
            if(GetLowestRoot(a,b,c,t,&NewT))
            {
                // NOTE(Banni): The velocity vector does intersect with the inifinite edge ray
                // Now we check if the intersection point lies in the edge segment
                f32 f = (EdgeDotVelocity * NewT - EdgeDotSpherePositionToVertex) / EdgeLengthSquared;
                if(f >= 0.0f && f <= 1.0f)
                {
                    t = NewT;
                    CollidingWithTriangle = true;
                    CollisionPoint = P1 + f * Edge;
                }
            }
            
            
            // NOTE(Banni): Edge (p2, p0)
            Edge = P0 - P2;
            SpherePositionToVertex = P2 - Position;
            
            EdgeLengthSquared = glm::length(Edge);
            EdgeLengthSquared *= EdgeLengthSquared;
            
            EdgeDotVelocity = glm::dot(Edge, Velocity);
            EdgeDotSpherePositionToVertex = glm::dot(Edge, SpherePositionToVertex);
            SpherePositionToVertexLengthSquared = glm::length(SpherePositionToVertex);
            SpherePositionToVertexLengthSquared *= SpherePositionToVertexLengthSquared;
            
            a = EdgeLengthSquared * -VelocityLengthSquared + (EdgeDotVelocity * EdgeDotVelocity);
            b = EdgeLengthSquared * (2.0f * glm::dot(Velocity, SpherePositionToVertex)) - (2.0f * EdgeDotVelocity * EdgeDotSpherePositionToVertex);
            c = EdgeLengthSquared * (1.0f - SpherePositionToVertexLengthSquared) + (EdgeDotSpherePositionToVertex * EdgeDotSpherePositionToVertex);
            
            if(GetLowestRoot(a,b,c,t,&NewT))
            {
                // NOTE(Banni): The velocity vector does intersect with the inifinite edge ray
                // Now we check if the intersection point lies in the edge segment
                f32 f = (EdgeDotVelocity * NewT - EdgeDotSpherePositionToVertex) / EdgeLengthSquared;
                if(f >= 0.0f && f <= 1.0f)
                {
                    t = NewT;
                    CollidingWithTriangle = true;
                    CollisionPoint = P2 + f * Edge;
                }
            }
            
        }
        
        if(CollidingWithTriangle)
        {
            f32 DistanceToCollision = t * glm::length(Velocity);
            if (Cp->FoundCollision == false || DistanceToCollision < Cp->NearestDistance)
            {
                Cp->NearestDistance = DistanceToCollision;
                Cp->IntersectionPoint = CollisionPoint;
                Cp->FoundCollision = true;
                return true;
            }
        }
        
    }
    return false;
}

b32 CheckPointIsInTriangle(const glm::vec3& Point,
                           const glm::vec3& P1,
                           const glm::vec3& P2,
                           const glm::vec3& P3)
{
    glm::vec3 CP1 = glm::cross((P3-P2), (Point-P2));
    glm::vec3 CP2 = glm::cross((P3-P2), (P1-P2));
    
    if(glm::dot(CP1, CP2) >= 0.0f)
    {
        CP1 = glm::cross((P3-P1), (Point-P1));
        CP2 = glm::cross((P3-P1), (P2-P1));
        if(glm::dot(CP1, CP2) >= 0.0f)
        {
            CP1 = glm::cross((P2-P1), (Point-P1));
            CP2 = glm::cross((P2-P1), (P3-P1));
            if(glm::dot(CP1, CP2) >= 0.0f)
            {
                return true;
            }
        }
    }
    return false;
}

b32 GetLowestRoot(f32 a, f32 b, f32 c, f32 maxR, f32* root) 
{
    // Check if a solution exists
    f32 determinant = b*b - 4.0f*a*c;
    // If determinant is negative it means no solutions.
    if (determinant < 0.0f) return false;
    // calculate the two roots: (if determinant == 0 then
    // x1==x2 but letï¿½ï¿½s disregard that slight optimization)
    f32 sqrtD = sqrt(determinant);
    f32 r1 = (-b - sqrtD) / (2*a);
    f32 r2 = (-b + sqrtD) / (2*a);
    // Sort so x1 <= x2
    if (r1 > r2) {
        f32 temp = r2;
        r2 = r1;
        r1 = temp;
    }
    // Get lowest root:
    if (r1 > 0 && r1 < maxR) {
        *root = r1;
        return true;
    }
    // It is possible that we want x2 - this can happen
    // if x1 < 0
    if (r2 > 0 && r2 < maxR) {
        *root = r2;
        return true;
    }

    // No (valid) solutions
    return false;
}