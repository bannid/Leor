f32 EaseInOutExponential(f32 t)
{
    if (t < 0.5f) {
        return pow(2.0f, 20.0f * t - 10.0f) * 0.5f;
    }
    return (2.0f - pow(2.0f, -20.0f * t + 10.0f)) * 0.5f;
}

f32 EaseOutBack(f32 t)
{
    f32 c1 = 1.70158f;
    f32 c3 = c1 + 1.0f;
    return 1.0f + c3 * pow(t - 1.0f, 3) + c1 * pow(t - 1.0f, 2);
}

f32 EaseInOutQuint(f32 t)
{
    if (t < 0.5f) {
        return  16.0f * pow(t, 5);
    }
    return 1.0f - pow(-2.0f * t + 2, 5) * 0.5f;
}

f32 EaseInBack(f32 t)
{
    f32 c1 = 1.70158f;
    f32 c3 = c1 + 1.0f;
    return c3 * t * t * t - c1 * t * t;
}

f32 EaseInOutSine(f32 t)
{
    return -(cos(PI * t) - 1.0f) / 2.0f;
}