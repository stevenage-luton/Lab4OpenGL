#version 460

in vec3 Vec;

out vec3 color;

layout(binding = 0) uniform sampler2D renderedTexture;
uniform float time;

void main(){
    vec2 UV = Vec.xy;
    color = texture( renderedTexture, UV + 0.005*vec2( sin(time+1024.0*UV.x),cos(time+768.0*UV.y)) ).xyz;
}