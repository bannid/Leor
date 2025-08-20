// NOTE(Banni): Macro to define a list struct
#define Declare_List(type) struct type##_list {\
u32 Size;\
u32 Length;\
b32 Initialized;\
type* Items;\
}

Declare_List(game_v2_interpolated);
