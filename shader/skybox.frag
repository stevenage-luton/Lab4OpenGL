#version 460

layout (binding = 0) uniform samplerCube CubeMap;
in vec3 Vec;
layout (location = 0) out vec4 FragColor;



void main() {
    vec4 bottomColour, topColour;
    bottomColour = vec4(0.5f,0.5f,0.5f,1.0f);
    topColour = vec4(0.0f,0.0f,0.0f,0.0f);

    // Calculate the vertical position from -1 (bottom) to 1 (top)
    float gradientFactor = (Vec.y + 1.0) / 50.0;

    // Interpolate between top and bottom color based on the gradient factor
    vec4 gradientColor = mix(bottomColour, topColour, gradientFactor);

   vec4 TexColour = texture(CubeMap, normalize(Vec));
   TexColour=pow(TexColour,vec4(1.0/2.2));

   // Mix the cubemap color with the gradient color
    vec4 finalColor = mix(gradientColor, TexColour, 1-gradientColor.a);

    FragColor = vec4(finalColor);
}
