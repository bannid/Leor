
v2 GetValue(game_v2_interpolated* Item)
{
    f32 t = Item->ElapsedTime / Item->TotalTime;
    if (t >= 1.0f)
    {
        Item->Value = Item->TargetValue;
        return Item->Value;
    }
    t = EaseInOutSine(t);
    return (1.0f - t) * Item->Value + t * Item->TargetValue;
}

void SetTargetValue(game_v2_interpolated* Item, v2 Value)
{
    Item->TargetValue = Value;
    Item->ElapsedTime = 0.0f;
}

void SetValue(game_v2_interpolated* Item, v2 Value, f32 AnimationTime)
{
    Item->TargetValue = Value;
    Item->Value = Value;
    Item->TotalTime = AnimationTime;
    Item->ElapsedTime = Item->TotalTime;
}