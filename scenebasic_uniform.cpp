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

#include "helper/texture.h"

using glm::vec3;

SceneBasic_Uniform::SceneBasic_Uniform() :
    tPrev(0),
    rotationSpeed(glm::pi<float>() / 2.0f),
    plane(50.0f,50.0f,1,1) 
    /*teapot(14,glm::mat4(1.0f))*/
    /*torus(1.75f * 0.75f, 1.75f * 0.75f,50,50)*/ {
    //mesh = ObjMesh::load("media/pig_triangulated.obj",true);
}

void SceneBasic_Uniform::initScene()
{
    compile();

    glEnable(GL_DEPTH_TEST);

    model = glm::mat4(1.0f);

    

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

    prog.setUniform("Light.L", vec3(1.0f));
    prog.setUniform("Light.La", vec3(0.05f));
    prog.setUniform("Fog.MaxDistance", 10.0f);
    prog.setUniform("Fog.MinDistance", 1.0f);
    prog.setUniform("Fog.Colour", vec3(0.5f,0.5f,0.5f));

    angle = 0.0f;

    brickTex = Texture::loadTexture("media/texture/cement.jpg");
    mossTex = Texture::loadTexture("media/texture/moss.png");
    fireTex = Texture::loadTexture("media/texture/fire.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brickTex);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mossTex);
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
    angle += 0.1f * deltaTime;
    if (this->m_animate)
    {
        angle += rotationSpeed * deltaTime;
        if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();

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

    glm::vec3 cameraPosition = glm::vec3(6.0f * cos(angle), 0.5f, 6.0f * sin(angle));
    view = glm::lookAt(cameraPosition, vec3(0.0f, 0.2f, 0.0f), vec3(0.0f, 1.0f, 0.0f));



    prog.setUniform("Light.Position", glm::vec4(0.0f,0.0f,0.0f,2.0f));


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brickTex);
    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    prog.setUniform("Material.Ks", vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("Material.Shininess", 2.0f);

    float dist = 0.0f;

    for (int i = 0; i < 5; i++)
    {

        model = glm::mat4(1.0f);
        model = glm::translate(model, vec3(0.0f, 0.5f, -dist));
        model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

        setMatrices();

        cube.render();
        dist += 2.0f;
    }

    //float dist = 0.0f;

    //for (int i = 0; i < 5; i++)
    //{
    //    model = glm::mat4(1.0f);
    //    model = glm::translate(model, vec3(dist*0.6f-1.0f, 0.0f, -dist));
    //    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

    //    setMatrices();

    //    teapot.render();

    //    dist += 7.0f;
    //}



    //prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    //prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    //prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    //prog.setUniform("Material.Shininess", 100.0f);

    //model = glm::mat4(1.0f);
    //model = glm::translate(model, vec3(-1.0f, -0.75f, 3.0f));
    //model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

    //setMatrices();

    //torus.render();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mossTex);
    prog.setUniform("Material.Kd", vec3(0.7f, 0.7f, 0.7f));
    prog.setUniform("Material.Ks", vec3(0.0f, 0.0f, 0.0f));
    prog.setUniform("Material.Ka", vec3(0.2f, 0.2f, 0.2f));
    prog.setUniform("Material.Shininess", 180.0f);

    model = glm::mat4(1.0f);

    setMatrices();

    plane.render();
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
