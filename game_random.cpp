f32 GetRandomNumber(game_random* Random)
{
    Random->Seed = (LARGE_MULTIPLIER * Random->Seed + 0) % LARGE_PRIME_NUMBER;
    return (f32)Random->Seed / (f32)(LARGE_PRIME_NUMBER - 1.0F);
}