#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (binding = 0) uniform sampler2D Texture;
layout (binding = 1) uniform sampler2D Overlay;
layout (location = 0) out vec4 FragColor;

uniform float Gamma = 1.4;

uniform bool OverlayToggle;

uniform struct LightInfo{
    vec4 Position;
    vec3 La; //Ambient
    vec3 L; //Diffuse & Specular
    vec3 Direction;
    float Exponent;
    float Cutoff;

}Lights [17];

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


vec3 blinnPhong(int light, vec3 position, vec3 normal){
    //Variables
    vec3 diffuse = vec3(0), specular = vec3(0);
    vec4 baseColour = texture(Texture,TexCoord);
    vec4 overlayColour = texture(Overlay,TexCoord);
    vec3 texColour = mix(baseColour.rgb, overlayColour.rgb, overlayColour.a);
    //Ambient Light
    vec3 ambient = Lights[light].La*texColour;

    //Diffuse
    vec3 lightToVertex = normalize(Lights[light].Position.xyz - position);

    float cosine = dot(-lightToVertex,normalize(Lights[light].Direction));
    float angle = acos(cosine);

    float spotScale;

    if  (angle>=0.0&&angle<Lights[light].Cutoff){
        spotScale = pow(cosine, Lights[light].Exponent);
        float dotProduct = max(dot(lightToVertex, normal),0.0);
        diffuse = texColour*dotProduct;

        if (dotProduct > 0.0){
            vec3 v = normalize(-position.xyz);
            vec3 h = normalize(v + lightToVertex);
            specular = Material.Ks*pow(max(dot(h,normal),0.0),Material.Shininess);
    
        }
    }

    


    //calculate Phong

    return ambient + spotScale*(diffuse + specular)*Lights[light].L;
}



void main() {
    vec4 alphaMap = texture(Texture, TexCoord);

    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDistance-dist)/(Fog.MaxDistance-Fog.MinDistance);
    fogFactor = clamp (fogFactor, 0.0, 1.0);

    vec3 shadeColour;

    for (int i=0;i<17;i++){
        if(alphaMap.a<0.15f){
        discard;
    }
    else{
        if (gl_FrontFacing){
            shadeColour +=blinnPhong(i,Position, normalize(Normal));
        }
        else{
            shadeColour +=blinnPhong(i,Position, normalize(-Normal));
        }
    }
        
    }

    

    

    vec3 finalColour = mix(Fog.Colour, shadeColour, fogFactor);
    


    FragColor = vec4(pow(finalColour, vec3(1.0/Gamma)), 1.0);
}
