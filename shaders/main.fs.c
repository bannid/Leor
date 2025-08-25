#version 330

in vec3 FragNormal;
in vec2 FragUv;

out vec4 FragColour;

uniform sampler2D uDiffuseTexture;
uniform bool uUseTexture;
void main()
{
    vec3 LightDir = normalize(vec3(1,1,-1));
    float Diffuse = clamp(.2, 1., dot(LightDir, FragNormal));
	FragColour = vec4(vec3(Diffuse), 1.0f);
    if(uUseTexture)
    {
        vec3 TextureColour = texture(uDiffuseTexture, FragUv * 4.0f).xyz;
        FragColour = vec4(TextureColour * Diffuse, 1.0f);
    }
}