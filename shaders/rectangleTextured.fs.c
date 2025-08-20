#version 330

in vec2 fTex;
out vec4 finalColour;

uniform sampler2D uTexture;

void main()
{
    vec4 col = texture(uTexture, fTex);
    finalColour = col;
}