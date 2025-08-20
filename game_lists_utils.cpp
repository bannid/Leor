// Generated code
void ResetList(game_v2_interpolated_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, game_v2_interpolated_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (game_v2_interpolated*)GetMemory(arena,Size * sizeof(game_v2_interpolated));
    List->Initialized = true;
}
    
game_v2_interpolated* InsertItem(game_v2_interpolated_list* List, game_v2_interpolated* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    game_v2_interpolated* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
game_v2_interpolated* GetItemPointer(game_v2_interpolated_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    game_v2_interpolated* ptr = List->Items + Index;
    return ptr;
}
    
game_v2_interpolated GetItem(game_v2_interpolated_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    game_v2_interpolated* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(game_v2_interpolated_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(game_v2_interpolated_list* List, game_v2_interpolated* Item)
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
    
void PopItemFront(game_v2_interpolated_list* List)
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
    
