#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "GLWindow.h"
#include "TextureLoader.h"
#include "CubeVertex.h"
#include "stb_image.h"



void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

int main() {
    GLWindow window = GLWindow(SCR_WIDTH, SCR_HEIGHT);

    window.use();

	Shader ourShader = Shader("./shaders/shader.vs", "./shaders/shader.fs");

    TextureLoader texttrueLoader = TextureLoader();
    texttrueLoader.loadNewAsset("glass_top", "./assets/textures/glass_top.png");
    texttrueLoader.loadNewAsset("glass_side", "./assets/textures/glass_side.png");
    texttrueLoader.loadNewAsset("glass_down", "./assets/textures/glass_down.png");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    CubeVertex cubeVertex = CubeVertex();

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f),
    };

    ourShader.use();
    glEnable(GL_DEPTH_TEST);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window.getWindow()))
    {
        // input
        // -----
        processInput(window.getWindow());

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(cubeVertex.VAO);

        for (unsigned int i = 0; sizeof(cubePositions) && i < int(sizeof(cubePositions) / sizeof(cubePositions[0])); i++)
        {
            // make sure to initialize matrix to identity matrix first
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            // model
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            // view
            // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
            view = glm::translate(view, glm::vec3(0.0f, -2.0f, -4.0f));

            // 透视视角
            projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);

            // retrieve the matrix uniform locations
            unsigned int modelLoc = glGetUniformLocation(ourShader.getID(), "model");
            unsigned int viewLoc = glGetUniformLocation(ourShader.getID(), "view");
            // pass them to the shaders (3 different ways)
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
            ourShader.setMat4("projection", projection);


            // render the triangle
            glBindTexture(GL_TEXTURE_2D, texttrueLoader.getAsset("glass_down"));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindTexture(GL_TEXTURE_2D, texttrueLoader.getAsset("glass_top"));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
            glBindTexture(GL_TEXTURE_2D, texttrueLoader.getAsset("glass_side"));
            glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}