// Generated code
void ResetList(timed_block_info_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, timed_block_info_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (timed_block_info*)GetMemory(arena,Size * sizeof(timed_block_info));
    List->Initialized = true;
}
    
timed_block_info* InsertItem(timed_block_info_list* List, timed_block_info* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    timed_block_info* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
timed_block_info* GetItemPointer(timed_block_info_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    timed_block_info* ptr = List->Items + Index;
    return ptr;
}
    
timed_block_info GetItem(timed_block_info_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    timed_block_info* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(timed_block_info_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(timed_block_info_list* List, timed_block_info* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    u32 Length = List->Length;
    for(int32 i = Length - 1; i >= 0; i--)
    {
        List->Items[i + 1] = List->Items[i];
    }
    List->Items[0] = *Item;
    List->Length++;
}
    
void PopItemFront(timed_block_info_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    if(List->Length == 0) return;
    u32 Length = List->Length;
    for(int32 i = 0; i < Length - 1; i++)
    {
        List->Items[i] = List->Items[i + 1];
    }
    List->Length--;
}
    
void ResetList(debug_variable_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, debug_variable_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (debug_variable*)GetMemory(arena,Size * sizeof(debug_variable));
    List->Initialized = true;
}
    
debug_variable* InsertItem(debug_variable_list* List, debug_variable* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    debug_variable* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
debug_variable* GetItemPointer(debug_variable_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    debug_variable* ptr = List->Items + Index;
    return ptr;
}
    
debug_variable GetItem(debug_variable_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    debug_variable* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(debug_variable_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(debug_variable_list* List, debug_variable* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    u32 Length = List->Length;
    for(int32 i = Length - 1; i >= 0; i--)
    {
        List->Items[i + 1] = List->Items[i];
    }
    List->Items[0] = *Item;
    List->Length++;
}
    
void PopItemFront(debug_variable_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    if(List->Length == 0) return;
    u32 Length = List->Length;
    for(int32 i = 0; i < Length - 1; i++)
    {
        List->Items[i] = List->Items[i + 1];
    }
    List->Length--;
}
    