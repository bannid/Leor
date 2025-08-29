void InitializeThirdPersonCamera(third_person_camera* Camera, f32 Distance)
{
    Camera->Distance = Distance;
    Camera->Yaw = .0f;
    Camera->Pitch = -10.0f;
    Camera->MinPitchDegrees = 70.0f;
    Camera->MaxPitchDegrees = -70.0f;
}
glm::quat GetOrientation(third_person_camera* Camera)
{
    glm::quat Yaw = glm::angleAxis(glm::radians(Camera->Yaw), glm::vec3(0,1,0));
    glm::quat Pitch = glm::angleAxis(glm::radians(Camera->Pitch), glm::vec3(1,0,0));
    return Yaw * Pitch;
}

glm::vec3 GetPosition(third_person_camera* Camera)
{
    glm::vec3 Forward = GetOrientation(Camera) * glm::vec3(0,0,-1);
    return Camera->Target - Forward * Camera->Distance;
}

glm::mat4 GetViewMatrix(third_person_camera* Camera)
{
    glm::vec3 Eye = GetPosition(Camera);
    glm::vec3 Forward = glm::normalize(Camera->Target - Eye);
    glm::vec3 Up = GetOrientation(Camera) * glm::vec3(0,1,0);
    return glm::lookAt(Eye, Camera->Target, Up);
}