//
// Created by head on 15.2.18.
//

#ifndef RENDER_MODEL_H
#define RENDER_MODEL_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../lib/stb_image.h"

#include "mesh.h"
#include "shader.h"

/// @brief  Represents set of meshes loaded from external file using assimp importer.
///         Enables to render meshes into active viewport.
///
/// Lorem ipsum. Bla bla, doxy.
class Model
{
public:
    /// @brief Constructs model form 3D scene file.
    /// \param path
    explicit Model(std::string path)  {
        loadModel(path);
    }

    /// @brief Renders model with current OpenGl configuration. Cool stuff.
    /// \param shader Shader class defines both vertex and fragment shader. Passed according to dependency inversion principle. Some additional info...
    ///
    /// Function calls Mesh::Draw of its mesh. See Mesh for more details.
    void Draw(Shader shader);

private:
    std::string directory;
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;

    /// @name Model loading functions
    /// This set of functions helps to load and process object files.
    /// @{

    /// @brief Load 3D scene using Assimp loader.
    /// \param path Relative path to object file.
    void loadModel(std::string path);

    /// @brief Takes individual node from scene and process its meshes.
    /// \param node Obejct node.
    /// \param scene 3D Scene loaded by loadModel().
    void processNode(aiNode *node, const aiScene *scene);

    /// @brief Process mesh. What would you expect?
    /// \param mesh
    /// \param scene
    /// \return Yeah... something.
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    /// @}

    /// @name Texture loading functions
    /// @{

    /// @brief It loads stuff to some vector or something.
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    /// @}
};

#endif //RENDER_MODEL_H
