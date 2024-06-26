#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include <glm/glm.hpp>

#include "helper/torus.h"
#include "helper/teapot.h"

#include <glm/gtc/matrix_transform.hpp>

#include "helper/plane.h"
#include "helper/objmesh.h"

#include "helper/cube.h"
#include <GLFW/glfw3.h>

#include "helper/skybox.h"

class GameObject {
private:

public:
    GameObject(std::string ourMesh, glm::vec3 ourPosition) {
        mesh = ourMesh;
        position = ourPosition;
    }


    std::string mesh;
    glm::vec3 position;
};

class SceneBasic_Uniform : public Scene
{
private:
    /*GLuint vaoHandle;*/
    GLSLProgram prog;
    GLSLProgram skyProg;
    GLSLProgram renderTexProg;

    float angle;


    SkyBox sky;
    //Torus torus;
    Plane plane;
    //Teapot teapot;
    Cube cube;

    std::unique_ptr<ObjMesh> RoadMesh, RoadMesh2, PavementMesh, WallMesh, ShopMesh, SideMesh, LampMesh;


    float tPrev;

    GLuint rustTex, roadTex, PavementTex, ShopTex, LampTex;

    GLuint fsQuad, renderTex,intermediateTex,renderFBO, intermediateFBO, depthTex;

    float rotationSpeed;

    glm::mat4 rotationMatrix;

    void compile();

    glm::vec3 cameraPosition = glm::vec3(3.0f, 3.0f, 3.0f);
    glm::vec3 cameraFront  = glm::vec3(0.0f, 0.0f, -1.0f);
    //Up position within world space
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float deltaTime;

    float moveSpeed = 3.5;

    void setupFBO();
    void pass1();
    void pass2();
    void pass3();

    float gauss(float, float);
    
public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void setMatrices(GLSLProgram &p);
    void setCameraRotation(glm::vec3 direction);
    void setCameraPosition(float x, float y, std::string direction);
    std::vector<GameObject> gameObjects;
    
};



#endif // SCENEBASIC_UNIFORM_H
