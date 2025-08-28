font_type OpenGL_FreeTypeLoadFontsFromMemoryToGPU(u8* Data, u32 Size, f32 PixelHeight)
{
    font_type Font = {};
    
    FT_Library FT;
    if(FT_Init_FreeType(&FT))
    {
        DEBUG_PRINT("ERROR: Failed to initialize free type");
        return Font;
    }
    
    FT_Face Face;
    if(FT_New_Memory_Face(FT, (FT_Byte*)Data, Size, 0, &Face))
    {
        DEBUG_PRINT("ERROR: Failed to read the Font file");
        return Font;
    }
    
    FT_Set_Pixel_Sizes(Face, 0, PixelHeight);
    
    //Disable the byte-alignment restrictions
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_GlyphSlot Slot = Face->glyph;
    for(u32 i = 0; i < NUMBER_OF_CHARACTERS; i++)
    {
        char C = (char)i;
        if(FT_Load_Char(Face, C, FT_LOAD_RENDER))
        {
            continue;
        }
        
        //FT_Render_Glyph(Slot, FT_RENDER_MODE_SDF);
        u32 TextureID;
        glGenTextures(1, &TextureID);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     Face->glyph->bitmap.width,
                     Face->glyph->bitmap.rows,
                     0,
                     GL_RED, 
                     GL_UNSIGNED_BYTE,
                     Face->glyph->bitmap.buffer
                     );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Font.Characters[i].TextureID = TextureID;
        Font.Characters[i].Size ={
            (int32)Face->glyph->bitmap.width,
            (int32)Face->glyph->bitmap.rows
        };
        Font.Characters[i].Bearing ={
            Face->glyph->bitmap_left,
            Face->glyph->bitmap_top
        };
        Font.Characters[i].Advance = (u32)Face->glyph->advance.x;
    }
    FT_Done_Face(Face);
    FT_Done_FreeType(FT);
    //Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    Font.Loaded = true;
    Font.PixelHeight = PixelHeight;
    return Font;
}

UsesScratchArena font_type 
FreeTypeLoadFontsFromFile(const char* FilePath,
                          f32 PixelHeight,
                          memory_arena Arena)
{
    u8* Buffer = (u8*)GetMemory(&Arena, MEGABYTE(1));
    int32 BytesRead = Win32ReadEntireFileV2(FilePath, Buffer, MEGABYTE(1));
    if (BytesRead)
    {
        return OpenGL_FreeTypeLoadFontsFromMemoryToGPU(Buffer, BytesRead, PixelHeight);
    }
    ASSERT_DEBUG(0);
    return {};
}