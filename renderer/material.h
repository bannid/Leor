/* date = September 4th 2025 9:24 pm */

#ifndef MATERIAL_H
#define MATERIAL_H

struct renderer_material
{
    u32 ShaderHandle;
    v4 Colour;
};

DECLARE_LIST(renderer_material);

#endif //MATERIAL_H
