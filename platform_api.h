/* date = August 2nd 2025 2:26 pm */

#ifndef PLATFORM_API_H
#define PLATFORM_API_H

typedef framebuffer layer;

#define Draw_Rectangle(Name) void Name(v3 Position, v2 Scale, v4 Colour)
typedef Draw_Rectangle(draw_rectangle);

#define Draw_Textured_Rectangle(Name) void Name(v3 Position, v2 Scale, u32 TextureID)
typedef Draw_Textured_Rectangle(draw_textured_rectangle);

#define Load_Texture(Name) u32 Name(const char* FilePath, int32 DesiredChannels, b32 Flip)
typedef Load_Texture(load_texture);

#define Create_Layer(Name) framebuffer Name(u32 Width, u32 Height)
typedef Create_Layer(create_layer);

#define Attach_Layer(Name) void Name(layer* Layer)
typedef Attach_Layer(attach_layer);

#define Clear_Attached_Layer(Name) void Name(v4 Colour)
typedef Clear_Attached_Layer(clear_attached_layer);

#define Draw_Text(Name) void Name(const char* Text, v3 Position, f32 PixelSize)
typedef Draw_Text(draw_text);

#define Measure_Text(Name) v2 Name(const char* Text, f32 PixelSize)
typedef Measure_Text(measure_text);

#define Int_To_String(Name) void Name(int32 Value, u8* Buffer, u32 BufferSize)
typedef Int_To_String(int_to_string);

#define Exit_Game(Name) void Name()
typedef Exit_Game(exit_game);

struct platform_api
{
    draw_rectangle*                    DrawRectangle;
    draw_text*                         DrawText;
    load_texture*                      LoadTexture;
    measure_text*                      MeasureText;
    draw_textured_rectangle*           DrawTexturedRectangle;
    create_layer*                      CreateLayer;
    attach_layer*                      AttachLayer;
    clear_attached_layer*              ClearAttachedLayer;
    int_to_string*                     IntToString;
    exit_game*                         ExitGame;
};

#endif //PLATFORM_API_H
