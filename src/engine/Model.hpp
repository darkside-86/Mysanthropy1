// Model.hpp
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2019
//-----------------------------------------------------------------------------
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see < https://www.gnu.org/licenses/>.
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "ogl/Program.hpp"
#include "ogl/Texture.hpp"

namespace engine
{
    // Loads and renders 3D models
    class Model
    {
    public:
        // constructor. Parameter is a 3D model file
        Model(const std::string& path);
        // destructor
        virtual ~Model();
        // TODO: better render method.
        void Draw(ogl::Program& program);
    private:
        void LoadModel(const std::string& path);
        Material LoadMaterial(aiMaterial* mat);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<ogl::Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, ogl::Texture::TYPE t);
        std::string directory_;
        std::vector<Mesh*> meshes_;
    };

}