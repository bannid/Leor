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
#include "material.h"

#define RENDERER_MAX_SHADERS                        200
#define RENDERER_DEFAULT_FONT_SIZE                  20
#define RENDERER_MAX_MATERIALS                      200

struct renderer
{
    GLFWwindow*                            Window;
    int32                                  Width;
    int32                                  Height;
    const char*                            WindowTitle;
    font_type                              Fonts;
    shader_program                         FontShader;
    u32                                    DefaultTexture;
    glm::mat4                              ScreenProjection;
    gl_model                               FontModel;
    shader_program_list                    Shaders;
    renderer_material_list                 Materials;
};

#endif //RENDERER_H
