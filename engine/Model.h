#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "ogl/Program.h"
#include "ogl/Texture.h"

namespace engine
{

    class Model
    {
    public:
        Model(const std::string& path);
        virtual ~Model();
        void Draw(ogl::Program& program);
    private:
        void LoadModel(const std::string& path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<ogl::Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, ogl::Texture::TYPE t);
        std::string directory_;
        std::vector<Mesh*> meshes_;
    };

}