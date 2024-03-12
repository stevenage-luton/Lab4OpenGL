#pragma once

#include <glad/glad.h>
#include "scene.h"
#include <GLFW/glfw3.h>
#include "glutils.h"

#define WIN_WIDTH 1800
#define WIN_HEIGHT 1200

#include <map>
#include <string>
#include <fstream>
#include <iostream>

class SceneRunner {
private:
    GLFWwindow * window;
    int fbw, fbh;
	bool debug;// Set true to enable debug messages
    //Relative position within world space
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    //The direction of travel
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    //Up position within world space
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    //Camera sidways rotation
    float cameraYaw = -90.0f;
    //Camera vertical rotation
    float cameraPitch = 0.0f;
    //Determines if first entry of mouse into window
    bool mouseFirstEntry = true;
    //Positions of camera from given last frame
    float cameraLastXPos = 800.0f / 2.0f;
    float cameraLastYPos = 600.0f / 2.0f;

public:
    SceneRunner(const std::string & windowTitle, int width = WIN_WIDTH, int height = WIN_HEIGHT, int samples = 0) : debug(true) {
        // Initialize GLFW
        if( !glfwInit() ) exit( EXIT_FAILURE );

#ifdef __APPLE__
        // Select OpenGL 4.1
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
#else
        // Select OpenGL 4.6
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
#endif
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        if(debug) 
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        if(samples > 0) {
            glfwWindowHint(GLFW_SAMPLES, samples);
        }

        // Open the window
        window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, windowTitle.c_str(), NULL, NULL );
        if( ! window ) {
			std::cerr << "Unable to create OpenGL context." << std::endl;
            glfwTerminate();
            exit( EXIT_FAILURE );
        }
        glfwMakeContextCurrent(window);

        // Get framebuffer size
        glfwGetFramebufferSize(window, &fbw, &fbh);

        // Load the OpenGL functions.
        if(!gladLoadGL()) { exit(-1); }

        GLUtils::dumpGLInfo();

        // Initialization
        glClearColor(0.5f,0.5f,0.5f,1.0f);
#ifndef __APPLE__
		if (debug) {
			glDebugMessageCallback(GLUtils::debugCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
			glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
				GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Start debugging");
		}
#endif
    }

    int run(Scene & scene) {
        scene.setDimensions(fbw, fbh);
        scene.initScene();
        scene.resize(fbw, fbh);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Enter the main loop
        mainLoop(window, scene);

#ifndef __APPLE__
		if( debug )
			glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 1,
				GL_DEBUG_SEVERITY_NOTIFICATION, -1, "End debug");
#endif

		// Close window and terminate GLFW
		glfwTerminate();

        // Exit program
        return EXIT_SUCCESS;
    }

    static std::string parseCLArgs(int argc, char ** argv, std::map<std::string, std::string> & sceneData) {
        if( argc < 2 ) {
            printHelpInfo(argv[0], sceneData);
            exit(EXIT_FAILURE);
        }

        std::string recipeName = argv[1];
        auto it = sceneData.find(recipeName);
        if( it == sceneData.end() ) {
            printf("Unknown recipe: %s\n\n", recipeName.c_str());
            printHelpInfo(argv[0], sceneData);
            exit(EXIT_FAILURE);
        }

        return recipeName;
    }

private:
    static void printHelpInfo(const char * exeFile,  std::map<std::string, std::string> & sceneData) {
        printf("Usage: %s recipe-name\n\n", exeFile);
        printf("Recipe names: \n");
        for( auto it : sceneData ) {
            printf("  %11s : %s\n", it.first.c_str(), it.second.c_str());
        }
    }

    void mainLoop(GLFWwindow * window, Scene & scene) {
        while( ! glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) ) {
            GLUtils::checkForOpenGLError(__FILE__,__LINE__);
			
            scene.update(float(glfwGetTime()));
            scene.setCameraRotation(HandleMouse(window));
            ProcessUserInput(window, scene);
            scene.render();

            
            glfwSwapBuffers(window);

            glfwPollEvents();
			/*int state = glfwGetKey(window, GLFW_KEY_SPACE);
			if (state == GLFW_RELEASE)
				scene.animate(!scene.animating());*/
        }
    }

    glm::vec3 HandleMouse(GLFWwindow* window) {
        GLdouble xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        //Initially no last positions, so sets last positions to current positions
        if (mouseFirstEntry)
        {
            cameraLastXPos = (float)xPos;
            cameraLastYPos = (float)yPos;
            mouseFirstEntry = false;
        }


        //Sets values for change in position since last frame to current frame
        float xOffset = (float)xPos - cameraLastXPos;
        float yOffset = cameraLastYPos - (float)yPos;

        //Sets last positions to current positions for next frame
        cameraLastXPos = (float)xPos;
        cameraLastYPos = (float)yPos;

        //Moderates the change in position based on sensitivity value
        const float sensitivity = 0.025f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        //Adjusts yaw & pitch values against changes in positions
        cameraYaw += xOffset;
        cameraPitch += yOffset;

        //Prevents turning up & down beyond 90 degrees to look backwards
        if (cameraPitch > 89.0f)
        {
            cameraPitch = 89.0f;
        }
        else if (cameraPitch < -89.0f)
        {
            cameraPitch = -89.0f;
        }

        //Modification of direction vector based on mouse turning
        glm::vec3 direction;
        direction.x = glm::cos(glm::radians(cameraYaw)) * glm::cos(glm::radians(cameraPitch));
        direction.y = glm::sin(glm::radians(cameraPitch));
        direction.z = glm::sin(glm::radians(cameraYaw)) * glm::cos(glm::radians(cameraPitch));
        return direction;
    }
    void ProcessUserInput(GLFWwindow* window, Scene& scene)
    {
        std::string dir;

        

        //WASD controls
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            dir = "FORWARD";
            scene.setCameraPosition(glm::sin(glm::radians(cameraYaw)), glm::cos(glm::radians(cameraYaw)),dir);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            dir = "BACK";
            scene.setCameraPosition(glm::sin(glm::radians(cameraYaw)), glm::cos(glm::radians(cameraYaw)), dir);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            dir = "LEFT";
            scene.setCameraPosition(glm::sin(glm::radians(cameraYaw)), glm::cos(glm::radians(cameraYaw)), dir);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            dir = "RIGHT";
            scene.setCameraPosition(glm::sin(glm::radians(cameraYaw)), glm::cos(glm::radians(cameraYaw)), dir);
        }

    }
};
