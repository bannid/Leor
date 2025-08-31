/* date = February 16th 2025 10:30 am */

#ifndef FONTS_H
#define FONTS_H

#define NUMBER_OF_CHARACTERS 128

struct font_character
{
    u32                                 TextureID;
    v2                                  Size;
    v2                                  Bearing;
    u32                                 Advance;
};

struct font_type
{
    font_character                      Characters[NUMBER_OF_CHARACTERS];
    f32                                 PixelHeight;
    b32                                 Loaded;
};

#endif //FONTS_H

