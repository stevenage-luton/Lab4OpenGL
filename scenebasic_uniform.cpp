#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

using glm::vec3;

SceneBasic_Uniform::SceneBasic_Uniform() :
    tPrev(0)
/*    plane(10.0f,10.0f,100,100), 
    teapot(14,glm::mat4(1.0f)),
    torus(1.75f * 0.75f, 1.75f * 0.75f,50,50)*/ {
    //mesh = ObjMesh::load("media/pig_triangulated.obj",true);
}

void SceneBasic_Uniform::initScene()
{
    compile();

    glEnable(GL_DEPTH_TEST);

    model = glm::mat4(1.0f);

    view = glm::lookAt(vec3(5.0f, 5.0f, 0.75f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    //model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f,0.0f,0.0f));
    //model = glm::rotate(model, glm::radians(15.0f), vec3(0.0f, 1.0f, 0.0f));

    projection = glm::mat4(1.0f);

    /*float x, z;
    for (int i = 0; i < 3; i++)
    {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);

        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
    }


    prog.setUniform("lights[0].L", vec3(0.0f, 0.0f, 0.8f));
    prog.setUniform("lights[1].L", vec3(0.0f, 0.8f, 0.0f));
    prog.setUniform("lights[2].L", vec3(0.8f, 0.0f, 0.0f));

    prog.setUniform("lights[0].La", vec3(0.0f, 0.0f, 0.2f));
    prog.setUniform("lights[1].La", vec3(0.0f, 0.2f, 0.0f));
    prog.setUniform("lights[2].La", vec3(0.2f, 0.0f, 0.0f));*/

    prog.setUniform("Spot.L", vec3(0.9f));
    prog.setUniform("Spot.La", vec3(0.5f));
    prog.setUniform("Spot.Exponent", vec3(50.0f));
    prog.setUniform("Spot.Cutoff", glm::radians(15.0f));

    angle = 0.0f;

    //std::cout << std::endl;

    //prog.printActiveUniforms();

    ///////////////////// Create the VBO ////////////////////
    //float positionData[] = {
    //    -0.8f, -0.8f, 0.0f,
    //     0.8f, -0.8f, 0.0f,
    //     0.0f,  0.8f, 0.0f };
    //float colorData[] = {
    //    1.0f, 0.0f, 0.0f,
    //    0.0f, 1.0f, 0.0f,
    //    0.0f, 0.0f, 1.0f };

    //// Create and populate the buffer objects
    //GLuint vboHandles[2];
    //glGenBuffers(2, vboHandles);
    //GLuint positionBufferHandle = vboHandles[0];
    //GLuint colorBufferHandle = vboHandles[1];

    //glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    //glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    //glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

    //// Create and set-up the vertex array object
    //glGenVertexArrays( 1, &vaoHandle );
    //glBindVertexArray(vaoHandle);

    //glEnableVertexAttribArray(0);  // Vertex position
    //glEnableVertexAttribArray(1);  // Vertex color

    //#ifdef __APPLE__
    //    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    //    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

    //    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    //    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
    //#else
    //		glBindVertexBuffer(0, positionBufferHandle, 0, sizeof(GLfloat)*3);
    //		glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat)*3);

    //		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    //		glVertexAttribBinding(0, 0);
    //		glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
    //	  glVertexAttribBinding(1, 1);
    //#endif
    //glBindVertexArray(0);
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    float deltaTime = t - tPrev;

    if (tPrev == 0.0f) 
    {
        deltaTime = 0.0f;
    }
    tPrev = t;
    angle += 0.25f * deltaTime;

    if (angle > glm::two_pi<float>())angle -= glm::two_pi<float>();
    {

    }

}

void SceneBasic_Uniform::setMatrices() {
    glm::mat4 mv = view * model;

    prog.setUniform("ModelViewMatrix", mv);

    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

    prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec4 lightPosition = glm::vec4(10.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
    prog.setUniform("Spot.Position", vec3(view*lightPosition));
    glm::mat3 normalMatrix = glm::mat3(glm::vec3(view[0]), glm::vec3(view[1]), glm::vec3(view[2]));
    prog.setUniform("Spot.Direction", normalMatrix*glm::vec3(-lightPosition));


    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    prog.setUniform("Material.Shininess", 100.0f);

    model = glm::mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.0f, -2.0f));
    model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

    setMatrices();

    cube.render();

    //prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    //prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    //prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    //prog.setUniform("Material.Shininess", 100.0f);

    //model = glm::mat4(1.0f);
    //model = glm::translate(model, vec3(-1.0f, -0.75f, 3.0f));
    //model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

    //setMatrices();

    //torus.render();

    //prog.setUniform("Material.Kd", vec3(0.7f, 0.7f, 0.7f));
    //prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
    //prog.setUniform("Material.Ka", vec3(0.2f, 0.2f, 0.2f));
    //prog.setUniform("Material.Shininess", 180.0f);

    //model = glm::mat4(1.0f);

    //setMatrices();

    //plane.render();
    //
    ////create the rotation matrix here and update the uniform in the shader 

    //rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));

    ////query the locaiton of the uniform handle

    //GLuint programHandle = prog.getHandle();

    //GLuint location = glGetUniformLocation(programHandle, "RotationMatrix");

    //glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);

    //glBindVertexArray(vaoHandle);
    //glDrawArrays(GL_TRIANGLES, 0, 3 );

    //glBindVertexArray(0);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}
