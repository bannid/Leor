// Generated code
void ResetList(entity_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, entity_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (entity*)GetMemory(arena,Size * sizeof(entity));
    List->Initialized = true;
}
    
entity* InsertItem(entity_list* List, entity* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    entity* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
entity* GetItemPointer(entity_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    entity* ptr = List->Items + Index;
    return ptr;
}
    
entity GetItem(entity_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    entity* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(entity_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(entity_list* List, entity* Item)
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
    
void PopItemFront(entity_list* List)
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
    
