#ifndef LISTS_H
#define LISTS_H

// NOTE(Banni): Macro to define a list struct
#define Declare_List(type) struct type##_list {\
u32 Size;\
u32 Length;\
b32 Initialized;\
type* Items;\
}

#define Declare_Named_Container(type) struct named_container_##type{\
char* Name;\
type Item;\
}

Declare_Named_Container(shader_program);
Declare_List(shader_program);
Declare_List(named_container_shader_program);

#endif