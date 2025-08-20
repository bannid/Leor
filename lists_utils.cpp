
        void ResetList(shader_program_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}

void InitList(memory_arena* arena, shader_program_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (shader_program*)GetMemory(arena,Size * sizeof(shader_program));
    List->Initialized = true;
}

shader_program* InsertItem(shader_program_list* List, shader_program* item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    shader_program* ptr = List->Items + List->Length++;
    *ptr = *item;
    return ptr;
}

shader_program* GetItemPointer(shader_program_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    shader_program* ptr = List->Items + Index;
    return ptr;
}

shader_program GetItem(shader_program_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    shader_program* ptr = List->Items + Index;
    return *ptr;
}

void DeInitList(shader_program_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}

