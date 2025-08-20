/* date = August 4th 2025 5:42 am */

#ifndef GAME_INTERPOLATEDS_H
#define GAME_INTERPOLATEDS_H

struct game_v2_interpolated
{
    v2 Value;
    v2 TargetValue;
    f32 ElapsedTime;
    f32 TotalTime;
};

#endif //GAME_INTERPOLATEDS_H
