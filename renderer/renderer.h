/* date = August 20th 2025 11:24 am */

#ifndef RENDERER_H
#define RENDERER_H

#include "opengl/framebuffer.h"
#include "opengl/model.h"
#include "opengl/shader.h"
#include "opengl/texture.h"
#include "fonts.h"
#include "model.h"
#include "scene.h"
#include "camera.h"

struct renderer
{
    GLFWwindow* Window;
    int32 Width;
    int32 Height;
    const char* WindowTitle;
    font_type Fonts;
};

#endif //RENDERER_H
