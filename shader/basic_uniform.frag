#version 460

in vec3 Position;
in vec3 Normal;
layout (location = 0) out vec4 FragColor;

uniform struct SpotLightInfo{
    vec3 Position;
    vec3 La; //Ambient
    vec3 L; //Diffuse & Specular
    vec3 Direction;
    float Exponent;
    float Cutoff;

}Spot;

uniform struct MaterialInfo{
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float Shininess;

}Material;


vec3 blinnPhongSpot(vec3 position, vec3 normal){
    vec3 diffuse = vec3(0), specular = vec3(0);

    //Ambient Light
    vec3 ambient = Spot.La*Material.Ka;

    //Diffuse
    vec3 lightToVertex = normalize(Spot.Position - position);

    float cosAng = dot(-lightToVertex,normalize(Spot.Direction));
    float angle = acos(cosAng);

    float spotScale;

    if (angle>=0.0&&angle<Spot.Cutoff){
        spotScale = pow(cosAng, Spot.Exponent);
        float dotProduct = max(dot(lightToVertex, normal),0.0);
        diffuse = Material.Kd*dotProduct;

        if (dotProduct > 0.0){
            vec3 v = normalize(-position.xyz);
            vec3 h = normalize(v + lightToVertex);
            specular = Material.Ks*pow(max(dot(h,normal),0.0),Material.Shininess);
    
        }
    }


    //calculate Phong

    return ambient + spotScale*(diffuse + specular)*Spot.L;
}


void main() {

    FragColor = vec4(blinnPhongSpot(Position, normalize(Normal)), 1.0);
}
