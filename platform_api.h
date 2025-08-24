/* date = August 2nd 2025 2:26 pm */

#ifndef PLATFORM_API_H
#define PLATFORM_API_H

#define Load_L_Model(Name) u32 Name(const char* Path);
typedef Load_L_Model(load_l_model);

#define Exit_Game(Name) void Name()
typedef Exit_Game(exit_game);

struct platform_api
{
    load_l_model*                          LoadLModel;
    exit_game*                             ExitGame;
};

#endif //PLATFORM_API_H
