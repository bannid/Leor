/* date = September 4th 2025 9:24 pm */

#ifndef MATERIAL_H
#define MATERIAL_H

#define DECLARE_RENDERER_MATERIAL_PROP(type) struct renderer_material_prop_##type{\
char* Name;\
type Value;\
}
DECLARE_RENDERER_MATERIAL_PROP(int32);
DECLARE_RENDERER_MATERIAL_PROP(v3);
DECLARE_RENDERER_MATERIAL_PROP(v4);

struct renderer_material
{
    u32 ShaderId;
    v4 Colour;
};

#endif //MATERIAL_H
