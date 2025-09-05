#ifndef DEBUG_INTERNAL_H
#define DEBUG_INTERNAL_H

#if defined(DEBUG)

struct timed_block_info
{
    const char* Name;
    f32 TimeTook;
};
DECLARE_LIST(timed_block_info);

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
    
    void End()
    {
        f32 TimeElapsed = glfwGetTime() - this->StartTime;
        timed_block_info T;
        T.TimeTook = TimeElapsed * 1000.0f;
        T.Name = this->BlockName;
        PushDebugTimingInfo(T);
    }
};


struct timed_function
{
    f32 StartTime;
    const char* FunctionName;
    timed_function(const char* FunctionName)
    {
        this->FunctionName = FunctionName;
        this->StartTime = glfwGetTime();
    }
    ~timed_function()
    {
        f32 TimeElapsed = glfwGetTime() - this->StartTime;
        timed_block_info T;
        T.TimeTook = TimeElapsed * 1000.0f;
        T.Name = this->FunctionName;
        PushDebugTimingInfo(T);
    }
};


struct debug_state
{
    b32 DrawCollisionMesh;
    b32 DrawDebugInfo;
};

// TODO(Banni): Maybe we can move this to debug.h?
enum debug_variable_type
{
    Debug_Variable_Type_Int32,
    Debug_Variable_Type_Float,
    Debug_Variable_Type_Bool,
    Debug_Variable_Type_V2,
    Debug_Variable_Type_V3,
    Debug_Variable_Type_V4
};

struct debug_variable
{
    const char* Name;
    debug_variable_type Type;
    void* Pointer;
};

DECLARE_LIST(debug_variable);

void DebugPushVariableToGlobal(const char* Name, debug_variable_type Type, void* Pointer);

#define TIMED_FUNCTION() timed_function T##__FUNCTION____FILE_____LINE__(##__FUNCTION__)

#define TIMED_BLOCK_START(name) timed_block T##name(#name)
#define TIMED_BLOCK_END(name) T##name.End()


#define DEBUG_PUSH_VARIABLE(Name, Type, Ptr) DebugPushVariableToGlobal(Name,Type,Ptr)
#else 

#define TIMED_FUNCTION()
#define DEBUG_PUSH_VARIABLE(Name, Type, Ptr)
#define TIMED_BLOCK_START(name) 
#define TIMED_BLOCK_END(name)
#endif

#endif