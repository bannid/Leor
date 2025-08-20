#version 330

in vec2 fTex;
out vec4 finalColour;

uniform sampler2D uTexture;

void main()
{
    vec2 tex = vec2(fTex.x, 1.0f - fTex.y);
    float sampled = texture(uTexture, tex).r;
    finalColour = vec4(1,1,1,sampled);
}