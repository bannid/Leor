/* date = August 21st 2025 0:35 pm */

#ifndef MODEL_H
#define MODEL_H

struct leor_vertex
{
    v3 Position;
    v3 Normal;
    v2 TextureCoords;
};
Declare_List(leor_vertex);
struct leor_mesh
{
    // TODO(Banni): We will just put these vertices in order maybe?
    // Or should we use EBO?
    leor_vertex_list Vertices;
};

#endif //MODEL_H
