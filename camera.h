#ifndef CAMERA_H
#define CAMERA_H
struct third_person_camera
{
    f32 Distance;
    f32 Yaw;
    f32 Pitch;
    f32 MinPitchDegrees;
    f32 MaxPitchDegrees;
    glm::vec3 Target;
};
#endif