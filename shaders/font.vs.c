#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

uniform mat4 uModel;
uniform mat4 uProjection;

out vec2 fTex;

void main()
{
    gl_Position = uProjection * uModel * vec4(aPos, 1.0f);
    fTex = aTex;
}