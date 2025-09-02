/* date = September 1st 2025 3:23 pm */

#ifndef DEBUG_UI_H
#define DEBUG_UI_H

struct debug_ui
{
    u32                                 Active;
    u32                                 Hot;
    v2                                  Cursor;
    u32_list                            ExpandedIDs;
};

#endif //DEBUG_UI_H
