#version 330

in vec3 FragNormal;
in vec2 FragUv;

out vec4 FragColour;

void main()
{
    vec3 LightDir = normalize(vec3(1,1,1));
    float Diffuse = clamp(.2, 1., dot(LightDir, FragNormal));
	FragColour = vec4(abs(FragNormal), 1.0f);
}