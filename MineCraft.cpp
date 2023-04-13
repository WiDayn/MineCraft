#include <GL/glew.h>
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
#include "Map.h"
#include "Player.h"



void processInput(GLFWwindow* window, Player player);
void updateGravity();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
std::vector<glm::vec3> flushVisiableChunk(Map map, Player player);
void openGLSetting(GLWindow window) {
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
}
void renderScene(Shader& shader, TextureLoader textrueLoader, std::vector<glm::vec3> cubePositions, Light light, GLuint depthMap, bool loadTexture);

float lastX = 400, lastY = 300;
bool firstMouse = true;
float yaw, pitch;
float y_speed;
float gravity = 10;
GLWindow window = GLWindow(800, 800);
Camera* camera;

int main() {
    window.use();

	Shader ourShader = Shader("./shaders/shader.vs", "./shaders/shader.fs");
    TextureLoader textrueLoader = TextureLoader();
    textrueLoader.loadNewAsset("glass_top", "./assets/textures/glass_top.png");
    textrueLoader.loadNewAsset("glass_side", "./assets/textures/glass_side.png");
    textrueLoader.loadNewAsset("glass_down", "./assets/textures/glass_down.png");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    CubeVertex cubeVertex = CubeVertex();

    
    Map map = Map();

    Player player = Player(0, 0, 3, 20);
    camera = &player.camera;
    
    LightVertex lightVertex = LightVertex();
    Light light = Light(0, 6, 0);

    openGLSetting(window);
    float last_time = 0;
    int tick = 0;
    std::vector<glm::vec3> cubePositions;

    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window.getWindow()))
    {
        if(tick++ % 10 == 0) cubePositions = flushVisiableChunk(map, player);

        float current_time = glfwGetTime();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;
        float fps = 1.0f / delta_time / 1000;
        std::cout << fps << "FPS" << std::endl;
        // input
        // -----
        processInput(window.getWindow(), player);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        GLfloat near_plane = 5.0f, far_plane = 100.0f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

        glm::mat4 lightView = glm::lookAt(light.lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        glViewport(0, 0, window.SCR_WIDTH, window.SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera->update(window.SCR_WIDTH, window.SCR_HEIGHT);
        glm::mat4 model = glm::mat4(1.0f);

        ourShader.use();
        glBindVertexArray(cubeVertex.VAO);
        ourShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        renderScene(ourShader, textrueLoader, cubePositions,  light, depthMap, true);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
        updateGravity();
        float sleepTime = 0;
        if (fps > 25) {
            sleepTime += 0.1;
        }
        else {
            sleepTime -= 0.1;
        }
        Sleep(sleepTime);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void renderScene(Shader &shader, TextureLoader textrueLoader, std::vector<glm::vec3> cubePositions, Light light, GLuint depthMap, bool loadTexture) {
    shader.setVec3("viewPos", camera->cameraPos);
    float last_time = 0;
    for (unsigned int i = 0; i < cubePositions.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[i]);
        // model
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(shader.getID(), "model");
        unsigned int viewLoc = glGetUniformLocation(shader.getID(), "view");
        unsigned int lightColorLoc = glGetUniformLocation(shader.getID(), "lightColor");

        shader.setMat4("model", model);
        shader.setMat4("view", camera->view);
        shader.setVec3("lightPos", light.lightPos);
        shader.setVec3("lightColor", light.lightColor);
        shader.setMat4("projection", camera->projection);

        // render the triangle
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textrueLoader.getAsset("glass_down"));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textrueLoader.getAsset("glass_top"));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textrueLoader.getAsset("glass_side"));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));
    }
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, Player player)
{
    float cameraSpeed = 0.4f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->cameraPos += cameraSpeed * camera->cameraFront * glm::vec3(1.0f, 0.0f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->cameraPos -= cameraSpeed * camera->cameraFront * glm::vec3(1.0f, 0.0f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->cameraPos -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->cameraPos += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && camera->cameraPos.y <= 3.01)
        camera->cameraPos.y += 1;
}

void updateGravity() {
    if (camera->cameraPos.y > 3) {
        camera->cameraPos.y -= float(y_speed);
        y_speed += gravity * (1e-10);
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
    camera->cameraFront = glm::normalize(front);
}

std::vector<glm::vec3> flushVisiableChunk(Map map, Player player) {
    int paintSize = 0;
    std::vector<glm::vec3> cubePositions = std::vector<glm::vec3>();
    int nowX = int(player.camera.cameraPos.x) / 16, nowZ = int(player.camera.cameraPos.z) / 16;
    std::cout << nowX << " " << nowZ << std::endl;
    for (int i = -paintSize; i <= paintSize; i++) {
        for (int j = -paintSize; j <= paintSize; j++) {
            Chunk* nowChunk = map.getChunk(nowX + i, nowZ + j);
            nowChunk->setXZ(nowX + i, nowZ + j);
            nowChunk->load();
            for (auto cube : map.getChunk(nowX + i, nowZ + j)->getCubeList()) {
                cubePositions.push_back(glm::vec3(float(nowChunk->getX() * 16 + cube.first.x), float(0 + cube.first.y), float(nowChunk->getZ() * 16 + cube.first.z)));
            }
        }
    }
    return cubePositions;
;}