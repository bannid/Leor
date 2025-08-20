/* date = August 8th 2025 6:13 pm */

#ifndef UI_H
#define UI_H

struct ui
{
    int32 Active;
    int32 Hot;
    
    v2 Cursor;
    platform_api* PlatformApi;
    mouse_input* Mouse;
    v2 MouseMapped;
};

#endif //UI_H
