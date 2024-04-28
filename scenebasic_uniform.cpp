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

const int LIGHT_NUMBER = 16;

SceneBasic_Uniform::SceneBasic_Uniform() :
    tPrev(0),
    rotationSpeed(glm::pi<float>() / 5.0f),
    plane(50.0f,50.0f,1,1),
    sky(100.0f)
    /*teapot(14,glm::mat4(1.0f))*/
    /*torus(1.75f * 0.75f, 1.75f * 0.75f,50,50)*/ {
    RoadMesh = ObjMesh::load("media/road.obj"),
    RoadMesh2 = ObjMesh::load("media/RoadNoLine.obj"),
    ShopMesh = ObjMesh::load("media/Shops.obj"),
    SideMesh = ObjMesh::load("media/Side.obj"),
    WallMesh = ObjMesh::load("media/Wall.obj"),
    LampMesh = ObjMesh::load("media/Lamp.obj"),
    PavementMesh = ObjMesh::load("media/Pavement.obj");
}

void SceneBasic_Uniform::initScene()
{
    compile();

    glEnable(GL_DEPTH_TEST);

    //model = glm::mat4(1.0f);

    
    

    //model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f,0.0f,0.0f));
    //model = glm::rotate(model, glm::radians(15.0f), vec3(0.0f, 1.0f, 0.0f));

    projection = glm::mat4(1.0f);
    float dist = -48.0f;

    for (int i = 0; i < 16; i++)
    {

        GameObject roadObj = GameObject("Road",vec3(0.0f,0.0f,dist));

        dist += 6.0f;

        gameObjects.push_back(roadObj);
    }
    dist = -48.0f;
    for (int i = 0; i < 16; i++)
    {

        GameObject pavementObj = GameObject("Pavement", vec3(0.0f, 0.0f, dist));

        dist += 6.0f;

        gameObjects.push_back(pavementObj);
    }
    dist = -96.0f;
    for (int i = 0; i < 8; i++)
    {

        GameObject lampObj = GameObject("Lamp", vec3(0.0f, 0.0f, dist));

        dist += 12.0f;

        gameObjects.push_back(lampObj);
    }

    dist = -48.0f;
    for (int i = 0; i < 8; i++)
    {

        GameObject WallObj = GameObject("Wall", vec3(0.0f, 0.0f, dist));
        GameObject SideObj = GameObject("Side", vec3(0.0f, 0.0f, dist));

        dist += 12.0f;

        gameObjects.push_back(WallObj);
        gameObjects.push_back(SideObj);
    }

    dist = 0.0f;
    GameObject ShopObj = GameObject("Shops", vec3(0.0f, 0.0f, dist));

    gameObjects.push_back(ShopObj);
   

    //prog.setUniform("Lights[0].L", vec3(1.9f, 1.9f, 01.9f));
    //prog.setUniform("Lights[1].L", vec3(1.9f, 1.9f, 01.9f));
    //prog.setUniform("Lights[2].L", vec3(1.9f, 1.9f, 01.9f));


    prog.setUniform("Lights[0].La", vec3(0.01f, 0.01f, 0.01f));


    /*prog.setUniform("Lights[0].Exponent", 200.0f);
    prog.setUniform("Lights[1].Exponent", 200.0f);
    prog.setUniform("Lights[2].Exponent", 200.0f);


    prog.setUniform("Lights[0].Cutoff", glm::radians(15.0f));
    prog.setUniform("Lights[1].Cutoff", glm::radians(15.0f));
    prog.setUniform("Lights[2].Cutoff", glm::radians(15.0f));*/

    for (int i = 0; i < LIGHT_NUMBER; i++)
    {

        std::stringstream LightVal, Exponent, Cutoff;
        LightVal << "Lights[" << i << "].L";

        Exponent << "Lights[" << i << "].Exponent";

        Cutoff << "Lights[" << i << "].Cutoff";

        prog.setUniform(LightVal.str().c_str(), vec3(1.9f, 1.9f, 1.9f));
        prog.setUniform(Exponent.str().c_str(), 75.0f);
        prog.setUniform(Cutoff.str().c_str(), glm::radians(40.0f));
    }
    prog.setUniform("Lights[16].L", vec3(2.0f, 1.0f, 1.0f));
    prog.setUniform("Lights[16].Exponent", 10.0f);
    prog.setUniform("Lights[16].Cutoff", glm::radians(50.0f));

    //prog.setUniform("Light.L", vec3(0.5f));
    //prog.setUniform("Light.La", vec3(0.01f));
    prog.setUniform("Fog.MaxDistance", 30.0f);
    prog.setUniform("Fog.MinDistance", 1.0f);
    prog.setUniform("Fog.Colour", vec3(0.059f, 0.078f, 0.106f));

    angle = 0.0f;

    GLuint skyBoxTex = Texture::loadCubeMap("media/texture/cube/sky/FullMoon");

    rustTex = Texture::loadTexture("media/texture/rust.png");
    roadTex = Texture::loadTexture("media/texture/road.png");
    PavementTex = Texture::loadTexture("media/texture/Pavement256.png");
    ShopTex = Texture::loadTexture("media/texture/ShopTex.png");
    LampTex = Texture::loadTexture("media/texture/Lamp.png");

    /*glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, rustTex);*/

    setupFBO();

    GLfloat verts[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };
    GLfloat tc[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    };

    unsigned int handle[2];
    glGenBuffers(2, handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

    glGenVertexArrays(1, &fsQuad);
    glBindVertexArray(fsQuad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    float weights[5], sum, sigma2 = 8.0f;

    weights[0] = gauss(0, sigma2);

    sum = weights[0];

    for (int i = 1; i < 5; i++)
    {
        weights[i] = gauss(float(i), sigma2);
        sum += 2 * weights[i];
    }

    for (int i = 1; i < 5; i++)
    {
        std::stringstream uniName;
        uniName << "Weight[" << i << "]";
        float val = weights[i] / sum;
        prog.setUniform(uniName.str().c_str(), val);
    }
}

void SceneBasic_Uniform::setupFBO() {
    glGenFramebuffers(1, &renderFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);

    glGenTextures(1, &renderTex);
    glBindTexture(GL_TEXTURE_2D, renderTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTex, 0);

    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);
    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (result == GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer is complete!" << endl;
    }
    else
    {
        std::cout << "Framebuffer Error: " << result << endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
        skyProg.compileShader("shader/skybox.vert");
        skyProg.compileShader("shader/skybox.frag");
        renderTexProg.compileShader("shader/passthrough.vert");
        renderTexProg.compileShader("shader/rendertex.frag");
        skyProg.link();
        renderTexProg.link();
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    deltaTime = t - tPrev;

    if (tPrev == 0.0f) 
    {
        deltaTime = 0.0f;
    }
    tPrev = t;
    if (this->m_animate)
    {
        angle += rotationSpeed * deltaTime;
        if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();

    }


}

void SceneBasic_Uniform::setMatrices(GLSLProgram &p) {
    glm::mat4 mv = view * model;

    p.setUniform("ModelViewMatrix", mv);

    p.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

    p.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::setCameraRotation(glm::vec3 direction) {
    cameraFront = normalize(direction);
};

void SceneBasic_Uniform::setCameraPosition(float x, float y, std::string direction) {
    x = x * moveSpeed * deltaTime;

    y = y * moveSpeed * deltaTime;

    //std::cout << "Xspeed = " << x << endl;

    //std::cout << "Yspeed = " << y << endl;

	if (direction == "FORWARD")
	{
		cameraPosition.x += y;
        cameraPosition.z += x;
	}
	else if (direction == "BACK")
	{
        cameraPosition.x -= y;
        cameraPosition.z -= x;
	}
	else if (direction == "LEFT")
	{
        cameraPosition.x += x;
        cameraPosition.z -= y;
	}
	else if (direction == "RIGHT")
	{
        cameraPosition.x -= x;
        cameraPosition.z += y;
	}

    if (cameraPosition.z >= 30.0f || cameraPosition.z <= -30.0f)
    {
        cameraPosition.z = 0.0f;
    }
};

void SceneBasic_Uniform::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
    pass1();
    glFlush();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    pass2();
    glFlush();
}

void SceneBasic_Uniform::pass1() {


    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(70.0f), (float)width / height, 0.3f, 100.0f);

    float lightDist = -47.0f;
    for (int i = 0; i < LIGHT_NUMBER; i++)
    {
        glm::vec4 Position;
        std::stringstream name;
        name << "Lights[" << i << "].Position";

        std::stringstream direction;
        direction << "Lights[" << i << "].Direction";
        if (i % 2 == 0)
        {
            Position = glm::vec4(4.0f, 16.0f, lightDist, 1.0f);
        }
        else
        {
            Position = glm::vec4(-4.0f, 16.0f, lightDist, 1.0f);
            lightDist += 12.0f;
        }


        glm::vec3 Direction = glm::vec3(0.0f, 10.0f, 0.0f);

        glm::mat3 normalMatrix = glm::mat3(glm::vec3(view[0]), vec3(view[1]), vec3(view[2]));

        prog.setUniform(name.str().c_str(), view * Position);


        prog.setUniform(direction.str().c_str(), normalMatrix * vec3(-Direction));


    }

    glm::vec4 redLightPos = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
    glm::vec3 Direction = glm::vec3(0.0f, 10.0f, 20.0f * sin(angle));
    prog.setUniform("Lights[16].Position", view * redLightPos);
    glm::mat3 normalMatrix = glm::mat3(glm::vec3(view[0]), vec3(view[1]), vec3(view[2]));
    prog.setUniform("Lights[16].Direction", normalMatrix * vec3(-Direction));
    //disable depth test and set the view to the normal matrix so the skybox stays far away
    view = normalMatrix;
    glDepthMask(GL_FALSE);
    skyProg.use();
    model = glm::mat4(1.0f);
    setMatrices(skyProg);
    sky.render();
    glDepthMask(GL_TRUE);
    view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    prog.use();


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, roadTex);
    prog.setUniform("Material.Kd", vec3(0.5f, 0.5f, 0.5f));
    prog.setUniform("Material.Ka", vec3(0.02f, 0.02f, 0.02f));
    prog.setUniform("Material.Ks", vec3(0.0f, 0.0f, 0.0f));
    prog.setUniform("Material.Shininess", 2000.0f);

    //float dist = 0.0f;

    //for (int i = 0; i < 5; i++)
    //{

    //    model = glm::mat4(1.0f);
    //    model = glm::translate(model, vec3(0.0f, 0.5f, -dist));
    //    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

    //    setMatrices();

    //    cube.render();
    //    dist += 2.0f;
    //}


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, roadTex);

    int num = 0;

    for (auto obj : gameObjects)
    {
        if (obj.mesh == "Road")
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, roadTex);

            model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);

            setMatrices(prog);

            if (num % 2 == 0)
            {
                RoadMesh->render();
            }
            else
            {
                RoadMesh2->render();
            }


        }
        else if (obj.mesh == "Pavement")
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, PavementTex);

            model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);

            setMatrices(prog);

            PavementMesh->render();
        }
        else if (obj.mesh == "Wall")
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, ShopTex);

            model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);

            setMatrices(prog);

            WallMesh->render();
        }
        else if (obj.mesh == "Shops")
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, ShopTex);

            model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);

            setMatrices(prog);

            ShopMesh->render();
        }
        else if (obj.mesh == "Side")
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, roadTex);

            model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);

            setMatrices(prog);

            SideMesh->render();
        }
        else if (obj.mesh == "Lamp")
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, LampTex);

            model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);

            setMatrices(prog);

            LampMesh->render();
        }

        num++;
    }
}

void SceneBasic_Uniform::pass2()
{

    glViewport(0, 0, width, height);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);

    renderTexProg.use();
    model = glm::mat4(1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);
    setMatrices(renderTexProg);


    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    glBindVertexArray(0);


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

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, mossTex);
    //prog.setUniform("Material.Kd", vec3(0.7f, 0.7f, 0.7f));
    //prog.setUniform("Material.Ks", vec3(0.0f, 0.0f, 0.0f));
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

void SceneBasic_Uniform::pass3()
{
    prog.setUniform("RenderTex", 0);
    prog.setUniform("Pass", 3);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, intermediateTex);
    glClear(GL_COLOR_BUFFER_BIT);


    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);

    setMatrices(prog);

    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glBindVertexArray(0);
}

float SceneBasic_Uniform::gauss(float x, float sigma2) {
    double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
    double exponent = -(x * x) / (2.0 * sigma2);

    return (float)(coeff * exp(exponent));
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}
