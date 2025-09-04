#version 330

in vec3 FragNormal;
in vec2 FragUv;
in vec3 FragWorld;

out vec4 FragColour;

uniform sampler2D uDiffuseTexture;
uniform bool uUseTexture;

void main()
{
    FragColour = vec4(FragNormal * .5 + .5, 1.0f);
}