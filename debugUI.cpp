inline void
MoveCursor(debug_ui* Ui, f32 Length)
{
    if(Ui->Flow == Debug_Ui_Flow_Horizontal)
    {
        Ui->Cursor += v2(Length, 0);
    }
    else
    {
        Ui->Cursor += v2(0, -Length);
    }
}

inline void
DebugUI_Label(debug_ui* Ui, const char* Label)
{
    DrawText(&GlobalRenderer,
             Ui->Cursor,
             Label,
             v4(1.0f));
    MoveCursor(Ui, 20);
}

b32
DebugUI_Button(debug_ui* Ui, const char* Label)
{
    f32 LabelWidth = MeasureTextWidthDefault(&GlobalRenderer, Label);
    f32 LabelHeight = GlobalRenderer.Fonts.PixelHeight;
    u32 Id = Utils_HashStringFNV1a(Label);
    input* Input = Ui->Input;
    b32 Result = false;
    
    if(Ui->Hot == Id)
    {
        DrawText(&GlobalRenderer, Ui->Cursor, Label, v4(1,1,0,1));
    }
    else
    {
        DrawText(&GlobalRenderer, Ui->Cursor, Label, v4(1));
    }
    
    if(Ui->Active == Id && Input->Mouse.Left.IsReleased && Ui->Hot == Id)
    {
        Result = true;
    }
    if(Ui->Hot == Id && Input->Mouse.Left.JustWentDown)
    {
        Ui->Active = Id;
    }
    
    if(Input->Mouse.Position.x > Ui->Cursor.x && Input->Mouse.Position.x < Ui->Cursor.x + LabelWidth &&
       Input->Mouse.Position.y > Ui->Cursor.y && Input->Mouse.Position.y < Ui->Cursor.y + LabelHeight)
    {
        Ui->Hot = Id;
    }
    else if(Ui->Hot == Id)
    {
        Ui->Hot = 0;
    }
    
    MoveCursor(Ui, 20);
    return(Result);
}

internal void
DrawDebugUI(debug_ui* Ui, input* Input)
{
    Ui->Cursor = v2(10, GlobalRenderer.Height - 20.0f);
    if(DebugUI_Button(Ui, "Collision mesh"))
    {
        GlobalDebugState.DrawCollisionMesh = !GlobalDebugState.DrawCollisionMesh;
    }
    
    if(DebugUI_Button(Ui, "Debug Info"))
    {
        GlobalDebugState.DrawDebugInfo = !GlobalDebugState.DrawDebugInfo;
    }
    
    if(GlobalDebugState.DrawDebugInfo)
    {
        char BufferToPrintStuff[256];
        // TODO(Banni): print out the debug info to the screen
        for (int32 i = 0; i < GlobalFrameTimesDebugInfo.Length; i++)
        {
            timed_block_info *DebugInfo = GetItemPointer(&GlobalFrameTimesDebugInfo, i);
            snprintf(BufferToPrintStuff, sizeof(BufferToPrintStuff), "%s: %.2f ms", DebugInfo->Name, DebugInfo->TimeTook);
            DebugUI_Label(Ui, (const char*)BufferToPrintStuff);
        }
        
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
                case Debug_Variable_Type_V2:
                {
                    v2* Pointer = (v2*)Variable->Pointer;
                    snprintf(BufferToPrintStuff, sizeof(BufferToPrintStuff),
                             "%s: (%.2f,%.2f)", Variable->Name, Pointer->x, Pointer->y);
                }break;
            }
            DebugUI_Label(Ui, (const char*)BufferToPrintStuff);
        }
    }
    ResetList(&GlobalFrameTimesDebugInfo);
    ResetList(&GlobalDebugVariableList);
}