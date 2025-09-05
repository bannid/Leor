#ifndef PRIMITIVES_H
#define PRIMITIVES_H

struct leor_primitive_triangle
{
    v3 V1;
    v3 V2;
    v3 V3;
};

DECLARE_LIST(leor_primitive_triangle);

#endif