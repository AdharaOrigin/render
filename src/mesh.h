//
// Created by head on 15.2.18.
//

#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include "shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

/// @brief Represents mesh object.
///
/// 3D object is represented as vector of verticies, related indicies and textures.
/// Every vertex holds information about its position, normal direction and texture coordinates.
class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    /// @brief Renders the mesh to binded viewport.
    /// \param shader Shader to be used.
    void Draw(Shader shader);
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif //RENDER_MESH_H
