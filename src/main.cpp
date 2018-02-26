#include <iostream>
#include <cmath>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../lib/stb_image.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <libgen.h>
#include <zconf.h>

#include "shader.h"
#include "objects.h"
#include "model.h"


GLFWwindow* create_context_window(int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void generateTexture(unsigned int& texture, const std::string& source_image_path, GLenum format, bool flip);
unsigned int loadTexture(std::string path);

const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

int main(int argc, char* argv[])
{
    std::string exe_path = dirname(argv[0]);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = create_context_window(SCR_WIDTH, SCR_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    Shader shader_basic(exe_path + "/resources/shaders/basic.vs", exe_path + "/resources/shaders/basic.fs");

    // VBO = buffer for sending data to GPU
    // VAO = object for storing data template (meaning of values in buffer)
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal vector attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



    unsigned int diffuseMap = loadTexture(exe_path + "/resources/textures/box_diff2.png");
    unsigned int specularMap = loadTexture(exe_path + "/resources/textures/box_spec.png");
    unsigned int stone = loadTexture(exe_path + "/resources/textures/stone_floor.jpg");

    // shader configuration
    // --------------------
    shader_basic.use();
    shader_basic.set_int("material.diffuse", 0);
    shader_basic.set_int("material.specular", 1);



    // Model crysis("/home/head/Development/Render/bin/resources/models/nanosuit/nanosuit.obj");
    Model sssh("/home/head/3D/modeling.obj");

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);


        float radius = 6.0f, offset = -2.0;
        float camX = sin(0.4 * glfwGetTime() - offset) * radius;
        float camZ = cos(0.4 * glfwGetTime() - offset) * radius;

        shader_basic.set_uniform("view_pos", glm::vec3(camX, 4.0, camZ));

        shader_basic.set_uniform("light.position", glm::vec3(2.0f, 2.0f, 2.0f));
        shader_basic.set_uniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        shader_basic.set_uniform("light.diffuse", glm::vec3(1.2f, 1.2f, 1.2f));
        shader_basic.set_uniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 model;
        glm::mat4 view = glm::lookAt(glm::vec3(camX, 2.0, camZ), glm::vec3(0.0, 0.55, 0.0), glm::vec3(0.0, 1.0, 0.0));
        shader_basic.set_uniform("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader_basic.set_uniform("projection", projection);


        shader_basic.set_int("material.shininess", 64);


        model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.3f));
        shader_basic.set_uniform("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // glDrawArrays(GL_TRIANGLES, 0, 36);


        model = glm::mat4();
        shader_basic.set_uniform("model", model);
        shader_basic.set_uniform("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, stone);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        // glDrawArrays(GL_TRIANGLES, 36, 42);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        shader_basic.set_uniform("light.ambient", glm::vec3(0.4f, 0.4f, 0.4f));
        shader_basic.set_int("material.shininess", 8);


        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        shader_basic.set_uniform("model", model);
        sssh.Draw(shader_basic);
/*
        model = glm::translate(model, glm::vec3(1.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        model = glm::rotate(model, 4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        shader_basic.set_uniform("model", model);
        crysis.Draw(shader_basic);
*/

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

/// @ingroup helpers
/// @brief Creates context window of defined width and height.
/// @return GLFW window object.
GLFWwindow *create_context_window(const int width, const int height) {

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw;
    }
    glfwMakeContextCurrent(window);

    // Init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw;
    }

    // Init drawing area & register callback function for window resize.
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/// @defgroup helpers Helper functions
/// This is set of helper functions used to:
/// - Process input
/// - Manipulate textures
///   + load textures
///   + generate textures
/// @{

/// @brief Le brief descr.
/// @param window window pointer
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void generateTexture(unsigned int &texture, const std::string& source_image_path, GLenum format, bool flip)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(source_image_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR::TEXTURE::Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

// model = glm::rotate(model, timeValue, glm::vec3(sin(0.5*timeValue)/2, 0.0f, 0.2f));

unsigned int loadTexture(std::string path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

/// @}