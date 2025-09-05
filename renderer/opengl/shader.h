/* date = July 29th 2025 6:40 pm */

#ifndef SHADER_H
#define SHADER_H

struct shader_program
{
    b32                 Valid;
    int32               ID;
    
    u32                 ProjectionLocation;
    u32                 ModelLocation;
    u32                 ColourLocation;
};

DECLARE_LIST(shader_program);

#endif //SHADER_H
