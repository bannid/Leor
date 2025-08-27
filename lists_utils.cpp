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
    
void ResetList(leor_vertex_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, leor_vertex_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (leor_vertex*)GetMemory(arena,Size * sizeof(leor_vertex));
    List->Initialized = true;
}
    
leor_vertex* InsertItem(leor_vertex_list* List, leor_vertex* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    leor_vertex* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
leor_vertex* GetItemPointer(leor_vertex_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    leor_vertex* ptr = List->Items + Index;
    return ptr;
}
    
leor_vertex GetItem(leor_vertex_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    leor_vertex* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(leor_vertex_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(leor_vertex_list* List, leor_vertex* Item)
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
    
void PopItemFront(leor_vertex_list* List)
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
    
void ResetList(leor_mesh_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, leor_mesh_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (leor_mesh*)GetMemory(arena,Size * sizeof(leor_mesh));
    List->Initialized = true;
}
    
leor_mesh* InsertItem(leor_mesh_list* List, leor_mesh* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    leor_mesh* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
leor_mesh* GetItemPointer(leor_mesh_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    leor_mesh* ptr = List->Items + Index;
    return ptr;
}
    
leor_mesh GetItem(leor_mesh_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    leor_mesh* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(leor_mesh_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(leor_mesh_list* List, leor_mesh* Item)
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
    
void PopItemFront(leor_mesh_list* List)
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
    
void ResetList(leor_model_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, leor_model_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (leor_model*)GetMemory(arena,Size * sizeof(leor_model));
    List->Initialized = true;
}
    
leor_model* InsertItem(leor_model_list* List, leor_model* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    leor_model* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
leor_model* GetItemPointer(leor_model_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    leor_model* ptr = List->Items + Index;
    return ptr;
}
    
leor_model GetItem(leor_model_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    leor_model* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(leor_model_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(leor_model_list* List, leor_model* Item)
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
    
void PopItemFront(leor_model_list* List)
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
    
void ResetList(leor_primitive_triangle_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, leor_primitive_triangle_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (leor_primitive_triangle*)GetMemory(arena,Size * sizeof(leor_primitive_triangle));
    List->Initialized = true;
}
    
leor_primitive_triangle* InsertItem(leor_primitive_triangle_list* List, leor_primitive_triangle* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    leor_primitive_triangle* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
leor_primitive_triangle* GetItemPointer(leor_primitive_triangle_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    leor_primitive_triangle* ptr = List->Items + Index;
    return ptr;
}
    
leor_primitive_triangle GetItem(leor_primitive_triangle_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    leor_primitive_triangle* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(leor_primitive_triangle_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(leor_primitive_triangle_list* List, leor_primitive_triangle* Item)
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
    
void PopItemFront(leor_primitive_triangle_list* List)
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
    