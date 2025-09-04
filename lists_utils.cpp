// Generated code
void ResetList(renderer_entity_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, renderer_entity_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (renderer_entity*)GetMemory(arena,Size * sizeof(renderer_entity));
    List->Initialized = true;
}
    
renderer_entity* InsertItem(renderer_entity_list* List, renderer_entity* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    renderer_entity* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
renderer_entity* GetItemPointer(renderer_entity_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    renderer_entity* ptr = List->Items + Index;
    return ptr;
}
    
renderer_entity GetItem(renderer_entity_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    renderer_entity* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(renderer_entity_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(renderer_entity_list* List, renderer_entity* Item)
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
    
void PopItemFront(renderer_entity_list* List)
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
    
void ResetList(u32_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, u32_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (u32*)GetMemory(arena,Size * sizeof(u32));
    List->Initialized = true;
}
    
u32* InsertItem(u32_list* List, u32* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    u32* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
u32* GetItemPointer(u32_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    u32* ptr = List->Items + Index;
    return ptr;
}
    
u32 GetItem(u32_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    u32* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(u32_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(u32_list* List, u32* Item)
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
    
void PopItemFront(u32_list* List)
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
    
void ResetList(int32_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, int32_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (int32*)GetMemory(arena,Size * sizeof(int32));
    List->Initialized = true;
}
    
int32* InsertItem(int32_list* List, int32* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    int32* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
int32* GetItemPointer(int32_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    int32* ptr = List->Items + Index;
    return ptr;
}
    
int32 GetItem(int32_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    int32* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(int32_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(int32_list* List, int32* Item)
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
    
void PopItemFront(int32_list* List)
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
    
void ResetList(f32_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    
void InitList(memory_arena* arena, f32_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (f32*)GetMemory(arena,Size * sizeof(f32));
    List->Initialized = true;
}
    
f32* InsertItem(f32_list* List, f32* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    f32* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    
f32* GetItemPointer(f32_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    f32* ptr = List->Items + Index;
    return ptr;
}
    
f32 GetItem(f32_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    f32* ptr = List->Items + Index;
    return *ptr;
}
    
void DeInitList(f32_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    
void InsertItemFront(f32_list* List, f32* Item)
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
    
void PopItemFront(f32_list* List)
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
    