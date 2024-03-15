#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (binding = 0) uniform sampler2D hdrTex;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 HdrColor;

uniform int Pass;
uniform float AverageLum;
uniform float Exposure = 0.4;
uniform float White = 0.928;
uniform bool ToneMapping = true;


uniform mat3 rgb2xyz=mat3(
0.4142564,0.2126729,0.0193339,
0.3575761,0.7151522,0.1191920,
0.1804375,0.0721740,0.9503041
);

uniform mat3 xyz2rgb=mat3(
3.2404542,-0.9692660,0.0556434,
-1.5371385,1.8760108,-0.2040259,
-0.4985314,0.0415560,1.05722252
);

uniform struct LightInfo{
    vec4 Position;
    vec3 La; //Ambient
    vec3 L; //Diffuse & Specular
    vec3 Direction;
    float Exponent;
    float Cutoff;

}Lights [16];

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
    vec3 texColour = texture(hdrTex,TexCoord).rgb;

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

void pass1(){
    HdrColor = vec3(0.0);

    vec4 alphaMap = texture(hdrTex, TexCoord);
    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDistance-dist)/(Fog.MaxDistance-Fog.MinDistance);
    fogFactor = clamp (fogFactor, 0.0, 1.0);

    vec3 shadeColour;

    for (int i=0;i<16;i++){
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

    HdrColor += finalColour;
}

void pass2(){
    vec4 colour = texture(hdrTex, TexCoord);
    vec3 xyzCol = rgb2xyz*vec3(colour);
    float xyzSum = xyzCol.x + xyzCol.y + xyzCol.z;
    vec3 xyYCol = vec3(xyzCol.x/xyzSum, xyzCol.y/xyzSum, xyzCol.y);

    float L = (Exposure * xyYCol.z)/AverageLum;
    L=(L*(1+L/(White*White)))/(1-L);
    xyzCol.x = (L*xyYCol.x)/xyYCol.y;
    xyzCol.y = L;
    xyzCol.z = (L*(1-xyYCol.x - xyYCol.y)) - xyYCol.y;

    if  (ToneMapping){
        FragColor = vec4(xyz2rgb*xyzCol, 1.0);
    }else{
        FragColor = colour;
    }

}

void main() {

    
    
    if  (Pass == 1) pass1();
    else if (Pass == 2) pass2();
}
