#version 460



layout (binding = 0) uniform samplerCube CubeMap;
in vec3 Vec;
layout (location = 0) out vec4 FragColor;



void main() {
   vec3 TexColour = texture(CubeMap, normalize(Vec)).rgb;

    FragColor = vec4(TexColour, 1.0);
}
