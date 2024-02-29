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

class SceneBasic_Uniform : public Scene
{
private:
    /*GLuint vaoHandle;*/
    GLSLProgram prog;
    float angle;

    //Torus torus;
    Plane plane;
    //Teapot teapot;
    Cube cube;

    //std::unique_ptr<ObjMesh> mesh;
    float tPrev;

    GLuint brickTex, mossTex, fireTex;

    float rotationSpeed;

    glm::mat4 rotationMatrix;

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void setMatrices();
};

#endif // SCENEBASIC_UNIFORM_H
