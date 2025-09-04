/* date = September 1st 2025 3:23 pm */

#ifndef DEBUG_UI_H
#define DEBUG_UI_H

enum debug_ui_flow
{
    Debug_Ui_Flow_Horizontal,
    Debug_Ui_Flow_Vertical
};

struct debug_ui
{
    u32                                 Active;
    u32                                 Hot;
    v2                                  Cursor;
    debug_ui_flow                       Flow;
    u32_list                            ExpandedIDs;
    input*                              Input;
};

#endif //DEBUG_UI_H
