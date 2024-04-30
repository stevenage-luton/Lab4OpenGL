#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (binding = 4) uniform sampler2D Texture;
layout (binding = 5) uniform sampler2D Overlay;
layout (location = 0) out vec4 FragColor;

uniform float Gamma = 1.4;

uniform bool OverlayToggle;

const float PI = 3.14159265359;

uniform struct LightInfo{
    vec4 Position;
    vec3 La; //Ambient
    vec3 L; //Diffuse & Specular
    vec3 Direction;
    float Exponent;
    float Cutoff;

}Lights [17];

uniform struct MaterialInfo{
    float Roughness;
    bool Metallic;

}Material;

uniform struct FogInfo{
    float MaxDistance;
    float MinDistance;
    vec3 Colour;

}Fog;


float distributionGGX(float nDotH) {
    float alpha2= Material.Roughness * Material.Roughness * Material.Roughness * Material.Roughness;
    float d = (nDotH * nDotH) * (alpha2 - 1) + 1;
    return alpha2 / (PI * d * d);

}

float geometrySmith(float dot){
    float k = (Material.Roughness + 1.0) * (Material.Roughness + 1.0) / 8.0;
    float denominator = dot * (1-k) + k;

    return 1.0 / denominator;
}

vec3 fresnelSchlick(float lDotH)
{
    vec3 F0 = vec3(0.04);
    if  (Material.Metallic){
        F0 = texture(Texture,TexCoord).rgb;
    }

    return F0 + (1.0 - F0) * pow(1.0 - lDotH, 5);
}  

vec3 microFacet(int light, vec3 position, vec3 normal){
    vec3 diffuse = vec3(0);
    if  (!Material.Metallic){
        diffuse = texture(Texture,TexCoord).rgb;
    }
    vec3 l = vec3(0.0), lightL = Lights[light].L;

   l = Lights[light].Position.xyz - position;
   float dist = length(l);
   l = normalize(l);
   lightL /= (dist * dist);


   vec3 v = normalize(-position);
   vec3 h = normalize(v + l);
   float nDotH = dot(normal,h);
   float lDotH = dot(l, h);
   float nDotL = max(dot(normal, l), 0.0);
   float nDotV = dot(normal, v);

   vec3 specular = 0.25 * distributionGGX(nDotH) * fresnelSchlick(lDotH) * geometrySmith(nDotL) * geometrySmith(nDotV);

   return (diffuse + PI * specular) * lightL * nDotL;

}




void main() {
    vec4 alphaMap = texture(Texture, TexCoord);

    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDistance-dist)/(Fog.MaxDistance-Fog.MinDistance);
    fogFactor = clamp (fogFactor, 0.0, 1.0);

    vec3 shadeColour = vec3(0);

    vec3 normalizedNormal = normalize(Normal);

    for (int i=0;i<17;i++){
        if(alphaMap.a<0.15f){
        discard;
    }
    else{
        if (gl_FrontFacing){
            shadeColour += microFacet(i, Position, normalizedNormal);
        }
        else{
            shadeColour += microFacet(i, Position, normalizedNormal);
        }
    }
        
    }

    

    

    vec3 finalColour = mix(Fog.Colour, shadeColour, fogFactor);
    


    FragColor = vec4(pow(finalColour, vec3(1.0/Gamma)), 1.0);
}
