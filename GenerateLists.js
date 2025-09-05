const fs = require('fs');

function ImplementInitFunction(typeName) {
    const functionDefinition = `
void InitList(memory_arena* arena, ${typeName}_list* List, u32 Size)
{
    ASSERT_DEBUG(!List->Initialized);
    List->Size = Size;
    List->Length = 0;
    List->Items = (${typeName}*)GetMemory(arena,Size * sizeof(${typeName}));
    List->Initialized = true;
}
    `;
    return functionDefinition;
}

function ImplementDeinitFunction(typeName) {
    const functionDefinition = `
void DeInitList(${typeName}_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Initialized = false;
}
    `;
    return functionDefinition;
}

function ImplementResetListFunction(typeName) {
    const functionDefinition = `
void ResetList(${typeName}_list* List)
{
    ASSERT_DEBUG(List->Initialized);
    List->Length = 0;
}
    `;
    return functionDefinition;
}

function ImplementInsertItemFunction(typeName) {
    const functionDefinition = `
${typeName}* InsertItem(${typeName}_list* List, ${typeName}* Item)
{
    ASSERT_DEBUG(List->Initialized);
    ASSERT_DEBUG(List->Length < List->Size);
    ${typeName}* ptr = List->Items + List->Length++;
    *ptr = *Item;
    return ptr;
}
    `;
    return functionDefinition;
}

function ImplementPushItemAtFrontFunction(typeName) {
    const functionDefinition = `
void InsertItemFront(${typeName}_list* List, ${typeName}* Item)
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
    `;
    return functionDefinition;
}

function ImplementPopItemFront(typeName) {
    const functionDefinition = `
void PopItemFront(${typeName}_list* List)
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
    `;
    return functionDefinition;
}

function ImplementGetItemFunction(typeName) {
    const functionDefinition = `
${typeName}* GetItemPointer(${typeName}_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    ${typeName}* ptr = List->Items + Index;
    return ptr;
}
    `;
    const functionDefinition2 = `
${typeName} GetItem(${typeName}_list* List,u32 Index)
{
    ASSERT_DEBUG(Index < List->Length);
    ${typeName}* ptr = List->Items + Index;
    return *ptr;
}
    `;
    return functionDefinition + functionDefinition2;
}

function GenerateSource(list)
{
    let source = '// Generated code';
    list.forEach((l) => {
        source += ImplementResetListFunction(l);
        source += ImplementInitFunction(l);
        source += ImplementInsertItemFunction(l);
        source += ImplementGetItemFunction(l);
        source += ImplementDeinitFunction(l);
        source += ImplementPushItemAtFrontFunction(l);
        source += ImplementPopItemFront(l);
    });
    return source;
}

function WriteLists() {
    const debugLists = [
        'timed_block_info',
        'debug_variable'
		
    ];
	const internalLists = [
		'shader_program'
	];
    const sharedLists = [
        'renderer_entity',
        'leor_vertex',
        'leor_mesh',
        'leor_model',
        'u32',
        'int32',
        'f32',
        'leor_primitive_triangle',
		'renderer_material'
    ];
    let sharedListsSource = GenerateSource(sharedLists);
    let debugListsSource = GenerateSource(debugLists);
	let internalListsSource = GenerateSource(internalLists);

    fs.writeFileSync('../lists_utils_debug.cpp', debugListsSource);
    fs.writeFileSync('../lists_utils.cpp', sharedListsSource);
	fs.writeFileSync('../lists_utils_internal.cpp', internalListsSource);
}
WriteLists();