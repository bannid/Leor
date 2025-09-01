#ifndef DEBUG_INTERNAL_H
#define DEBUG_INTERNAL_H

#if defined(DEBUG)

struct timed_block_info
{
    const char* Name;
    f32 TimeTook;
};
Declare_List(timed_block_info);

extern void PushDebugTimingInfo(timed_block_info);

struct timed_block
{
    f32 StartTime;
    const char* BlockName;
    timed_block(const char* BlockName)
    {
        this->BlockName = BlockName;
        this->StartTime = glfwGetTime();
    }
    ~timed_block()
    {
        f32 TimeElapsed = glfwGetTime() - this->StartTime;
        timed_block_info T;
        T.TimeTook = TimeElapsed * 1000.0f;
        T.Name = this->BlockName;
        PushDebugTimingInfo(T);
    }
};

struct debug_state
{
    b32 DrawCollisionMesh;
    b32 DrawTimeUtils;
};

enum debug_variable_type
{
    Debug_Variable_Type_Int,
    Debug_Variable_Type_Float,
    Debug_Variable_Type_Bool,
    Debug_Variable_Type_V3,
    Debug_Variable_Type_V4
};

struct debug_variable
{
    const char* Name;
    debug_variable_type Type;
    void* Pointer;
};

Declare_List(debug_variable);

void DebugPushVariableToGlobal(const char* Name, debug_variable_type Type, void* Pointer);

#define TIMED_BLOCK(x) timed_block T##__FUNCTION____FILE_____LINE__(x)
#define DEBUG_PUSH_VARIABLE(Name, Type, Ptr) DebugPushVariableToGlobal(Name,Type,Ptr)
#else 
#define TIMED_BLOCK(x)
#define DEBUG_PUSH_VARIABLE(Name, Type, Ptr)
#endif

#endif