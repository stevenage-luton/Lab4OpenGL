#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (binding = 0) uniform sampler2D Texture;
layout (location = 0) out vec4 FragColor;

uniform struct LightInfo{
    vec4 Position;
    vec3 La; //Ambient
    vec3 L; //Diffuse & Specular

}Light;

uniform struct MaterialInfo{
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float Shininess;

}Material;

uniform struct FogInfo{
    float MaxDistance;
    float MinDistance;
    vec3 Colour;

}Fog;


vec3 blinnPhong(vec3 position, vec3 normal){
    //Variables
    vec3 diffuse = vec3(0), specular = vec3(0);
    vec3 texColour = texture(Texture,TexCoord).rgb;

    //Ambient Light
    vec3 ambient = Light.La*texColour;

    //Diffuse
    vec3 lightToVertex = normalize(Light.Position.xyz - position);

    float dotProduct = max(dot(lightToVertex, normal),0.0);
    diffuse = texColour*dotProduct;

    if (dotProduct > 0.0){
        vec3 v = normalize(-position.xyz);
        vec3 h = normalize(v + lightToVertex);
        specular = Material.Ks*pow(max(dot(h,normal),0.0),Material.Shininess);
    
    }


    //calculate Phong

    return ambient + (diffuse + specular)*Light.L;
}


void main() {
    vec4 alphaMap = texture(Texture, TexCoord);
    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDistance-dist)/(Fog.MaxDistance-Fog.MinDistance);
    fogFactor = clamp (fogFactor, 0.0, 1.0);

    vec3 shadeColour;

    if(alphaMap.a<0.15f){
        discard;
    }
    else{
        if (gl_FrontFacing){
            shadeColour =blinnPhong(Position, normalize(Normal));
        }
        else{
            shadeColour =blinnPhong(Position, normalize(-Normal));
        }
    }

    

    vec3 finalColour = mix(Fog.Colour, shadeColour, fogFactor);
    


    FragColor = vec4(finalColour, 1.0);
}
