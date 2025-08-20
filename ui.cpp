#define BUTTON_COLOUR v4(0.61, 0.13, 0.15, 1.)

void BeginUI(ui* Gui, v2 Position)
{
    Gui->Cursor = Position;
}

bool UI_Button(const char* Text, ui* Gui, int32 Id)
{
    b32 Result = false;
    v2 Size = v2(150, 40);
    f32 TextSizePx = 15.0f;
    f32 Shadow = 6.0f;
    f32 Spacing = 10.0f;
    
    v2 MousePosition = Gui->MouseMapped;
    
    b32 MouseOver = (MousePosition.x > Gui->Cursor.x && MousePosition.x < Gui->Cursor.x + Size.x) &&
    (MousePosition.y > Gui->Cursor.y && MousePosition.y < Gui->Cursor.y + Size.y);
    
    if(Gui->Hot == Id)
    {
        Gui->PlatformApi->DrawRectangle(v3(Gui->Cursor, .0f),
                                        Size,
                                        BUTTON_COLOUR * 1.1f);
    }
    else
    {
        Gui->PlatformApi->DrawRectangle(v3(Gui->Cursor, .0f), Size, BUTTON_COLOUR);
    }
    
    if(MouseOver)
    {
        Gui->Hot = Id;
    }
    else if(Gui->Hot == Id)
    {
        Gui->Hot = 0;
    }
    
    Gui->PlatformApi->DrawRectangle(v3(Gui->Cursor - (Shadow * .5f), .0f),
                                    Size + Shadow, col4(.0, .0, .0, 1.));
    
    v2 TextSize = Gui->PlatformApi->MeasureText(Text, TextSizePx);
    v2 TextPosition = Gui->Cursor + Size * .5f - TextSize * .5f;
    Gui->PlatformApi->DrawText(Text, v3(TextPosition, -1.0f), TextSizePx);
    
    if(Gui->Mouse->Left.IsReleased
       && Gui->Active == Id
       && MouseOver)
    {
        Result = true;
        Gui->Active = 0;
    }
    
    if(Gui->Mouse->Left.JustWentDown && Gui->Hot == Id)
    {
        Gui->Active = Id;
    }
    
    Gui->Cursor.y += Size.y + Spacing;
    return Result;
}
