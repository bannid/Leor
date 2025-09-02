internal void
DrawDebugUI()
{
    char BufferToPrintStuff[256];
    f32 YOffset = 0.0f;
    // TODO(Banni): print out the debug info to the screen
    for (int32 i = 0; i < GlobalFrameTimesDebugInfo.Length; i++)
    {
        timed_block_info *DebugInfo = GetItemPointer(&GlobalFrameTimesDebugInfo, i);
        snprintf(BufferToPrintStuff, sizeof(BufferToPrintStuff), "%s: %.2f ms", DebugInfo->Name, DebugInfo->TimeTook);
        DrawText(&GlobalRenderer, glm::vec2(10.0f, GlobalRenderer.Height - 20.0f - YOffset), BufferToPrintStuff);
        YOffset += 20.0f;
    }
    ResetList(&GlobalFrameTimesDebugInfo);
    
    YOffset += 50.0f;
    
    // TODO(Banni): print out the debug info to the screen
    for (int32 i = 0; i < GlobalDebugVariableList.Length; i++)
    {
        debug_variable *Variable = GetItemPointer(&GlobalDebugVariableList, i);
        switch(Variable->Type)
        {
            case Debug_Variable_Type_V3:
            {
                v3* Pointer = (v3*)Variable->Pointer;
                snprintf(BufferToPrintStuff, sizeof(BufferToPrintStuff),
                         "%s: (%.2f,%.2f,%.2f)", Variable->Name, Pointer->x, Pointer->y, Pointer->z);
            }break;
        }
        DrawText(&GlobalRenderer, glm::vec2(10.0f, GlobalRenderer.Height - 20.0f - YOffset), BufferToPrintStuff);
        YOffset += 20.0f;
    }
    ResetList(&GlobalDebugVariableList);
}