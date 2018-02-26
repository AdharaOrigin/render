//
// Created by head on 12.2.18.
//

#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/// @brief Wrapper class for vertex and fragment shaders.
///
/// Constructor loads and compiles shaders from specified files. Overloaded function set_uniform() allows further
/// shader configuration.
class Shader {
public:
    unsigned int ID;
    Shader(std::string vertexSource, std::string fragmentSource);

    /// @brief Binds shader to OpenGl context.
    void use();

    /// @brief Sets uniform to shader.
    void set_int(const std::string &name, int value) const;

    /// @brief Sets uniform to shader.
    /// @overload void set_uniform(const std::string &name, float value) const
    void set_uniform(const std::string &name, float value) const;

    /// @overload void set_uniform(const std::string &name, const glm::vec3 &vec) const;
    void set_uniform(const std::string &name, const glm::vec3 &vec) const;

    /// @overload void set_uniform(const std::string &name, const glm::mat4 &mat) const
    void set_uniform(const std::string &name, const glm::mat4 &mat) const;

private:
    bool copy_file_content(std::string path, std::string &content);
    bool shader_compiled(const unsigned int &shader);
    bool program_linked(const unsigned int &program);
};


#endif //RENDER_SHADER_H
