#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "GLWindow.h"
#include "TextureLoader.h"
#include "CubeVertex.h"
#include "stb_image.h"
#include "Chunk.h"
#include "Camera.h"
#include "LightVertex.h"
#include "Light.h"



void processInput(GLFWwindow* window);
void updateGravity();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float lastX = 400, lastY = 300;
bool firstMouse = true;
float yaw, pitch;
float y_speed;
float gravity = 10;
GLWindow window = GLWindow(600, 600);
Camera camera = Camera();

int main() {
    window.use();

	Shader ourShader = Shader("./shaders/shader.vs", "./shaders/shader.fs");
    Shader lightShader = Shader("./shaders/shader.vs", "./shaders/light.fs");
    TextureLoader texttrueLoader = TextureLoader();
    texttrueLoader.loadNewAsset("glass_top", "./assets/textures/glass_top.png");
    texttrueLoader.loadNewAsset("glass_side", "./assets/textures/glass_side.png");
    texttrueLoader.loadNewAsset("glass_down", "./assets/textures/glass_down.png");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    CubeVertex cubeVertex = CubeVertex();

    std::vector<glm::vec3> cubePositions = std::vector<glm::vec3>();
    
    Chunk chunk1 = Chunk(-15, -15);
    Chunk chunk2 = Chunk(0, -15);
    chunk1.load();
    chunk2.load();
    for (auto& cube : chunk1.getCubeList()) {
        cubePositions.push_back(glm::vec3(float(chunk1.getX() + cube.first.x), float(0 + cube.first.y), float(chunk1.getZ() + cube.first.z)));
    }
    for (auto& cube : chunk2.getCubeList()) {
        cubePositions.push_back(glm::vec3(float(chunk2.getX() + cube.first.x), float(0 + cube.first.y), float(chunk2.getZ() + cube.first.z)));
    }

    LightVertex lightVertex = LightVertex();
    Light light = Light(5, 5, -10);

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    float last_time = 0;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window.getWindow()))
    {
        float current_time = glfwGetTime();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;
        float fps = 1.0f / delta_time;
        std::cout << fps << std::endl;
        // input
        // -----
        processInput(window.getWindow());

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.update(window.SCR_WIDTH, window.SCR_HEIGHT);
        glm::mat4 model = glm::mat4(1.0f);


        lightShader.use();
        glBindVertexArray(lightVertex.VAO);
        // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, light.lightPos);

        lightShader.setMat4("model", model);
        lightShader.setMat4("view", camera.view);
        lightShader.setMat4("projection", camera.projection);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        ourShader.use();
        glBindVertexArray(cubeVertex.VAO);
        float last_time = 0;
        for (unsigned int i = 0; i < cubePositions.size(); i++)
        {   
            model = glm::mat4(1.0f);
            // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            // model
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            // retrieve the matrix uniform locations
            unsigned int modelLoc = glGetUniformLocation(ourShader.getID(), "model");
            unsigned int viewLoc = glGetUniformLocation(ourShader.getID(), "view");
            unsigned int lightColorLoc = glGetUniformLocation(ourShader.getID(), "lightColor");
            // pass them to the shaders (3 different ways)
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera.view[0][0]);
           
            ourShader.setVec3("lightPos", light.lightPos);
            ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            ourShader.setMat4("projection", camera.projection);

            // render the triangle
            glBindTexture(GL_TEXTURE_2D, texttrueLoader.getAsset("glass_down"));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindTexture(GL_TEXTURE_2D, texttrueLoader.getAsset("glass_top"));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
            glBindTexture(GL_TEXTURE_2D, texttrueLoader.getAsset("glass_side"));
            glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));
        
            updateGravity();
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
        Sleep(50);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    float cameraSpeed = 0.25f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.cameraPos += cameraSpeed * camera.cameraFront * glm::vec3(1.0f, 0.0f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.cameraPos -= cameraSpeed * camera.cameraFront * glm::vec3(1.0f, 0.0f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && camera.cameraPos.y <= 3.01)
        camera.cameraPos.y += 1;
}

void updateGravity() {
    if (camera.cameraPos.y > 3) {
        camera.cameraPos.y -= float(y_speed);
        y_speed += gravity * (1e-8);
    } else {
        y_speed = 0;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera.cameraFront = glm::normalize(front);
}