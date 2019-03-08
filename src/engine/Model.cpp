// Model.cpp 
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

#include "engine/GameEngine.hpp"
#include "Model.hpp"

namespace engine
{

    Model::Model(const std::string& path)
    {
        LoadModel(path);
    }

    Model::~Model()
    {
        // we have ownership of Texture pointers created from loading model files
        for(auto mesh : meshes_)
        {
            mesh->DestroyTextures();
            delete mesh;
        }
    }

    void Model::Draw(ogl::Program& program)
    {
        for(auto mesh : meshes_)
        {
            mesh->Draw(program);
        }
    }

    Material Model::LoadMaterial(aiMaterial* mat)
    {
        Material material;
        aiColor3D color;
        float shininess;
        mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        material.diffuse = {color.r, color.g, color.b};
        mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
        material.specular = {color.r, color.g, color.b};
        mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
        material.ambient = {color.r, color.g, color.b};
        mat->Get(AI_MATKEY_SHININESS, shininess);
        material.shininess = shininess;
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
                "Diffuse=%f %f %f\n" 
                "Specular=%f %f %f\n"
                "Ambient=%f %f %f\n"
                "Shininess=%f", 
                material.diffuse.r, material.diffuse.g, material.diffuse.b,
                material.specular.r, material.specular.g, material.specular.b,
                material.ambient.r, material.ambient.g, material.ambient.b,
                material.shininess);
        return material;
    }

    void Model::LoadModel(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if((scene == nullptr) || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
                "%s: %s", __FUNCTION__, importer.GetErrorString());
            return;
        }
        directory_ = path.substr(0, path.find_last_of('/'));
        ProcessNode(scene->mRootNode, scene);
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        // node's meshes
        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes_.push_back(ProcessMesh(mesh, scene));
        }
        // its children
        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }
        
    Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<ogl::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<ogl::Texture*> textures;
        Material mat;

        // process vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            ogl::Vertex vertex;
            // position
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
            // color : white
            vertex.color = {255,255,255,255};
            // normal
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
            // texcoords (only consider the first of a possible set)
            if(mesh->mTextureCoords[0])
            {
                vertex.texCoords.s = mesh->mTextureCoords[0][i].x;
                vertex.texCoords.t = mesh->mTextureCoords[0][i].y;
            }
            else 
            {
                vertex.texCoords = {0.f, 0.f};
            }
            vertices.push_back(vertex);
        }
        // process indices
        for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; ++j)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        // process textures
        if(mesh->mMaterialIndex >= 0)
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::INFO, 
                    "materialIndex=%d", mesh->mMaterialIndex);
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<ogl::Texture*> diffuseMaps = LoadMaterialTextures(material, 
                    aiTextureType_DIFFUSE, ogl::Texture::TYPE::DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<ogl::Texture*> specularMaps = LoadMaterialTextures(material,
                    aiTextureType_SPECULAR, ogl::Texture::TYPE::SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            mat = LoadMaterial(material);
        }

        return new Mesh(vertices, indices, textures, mat);
    }

    std::vector<ogl::Texture*> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, ogl::Texture::TYPE t)
    {
        std::vector<ogl::Texture*> textures;
        for(unsigned int i=0; i < mat->GetTextureCount(type); ++i)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            ogl::Texture* texture = new ogl::Texture(directory_ + "/" + std::string(str.C_Str()), true, t);
            GameEngine::Get().GetLogger().Logf(Logger::Severity::INFO, "Loading texture %s of type %d", 
                    str.C_Str(), (int)t);
            textures.push_back(texture);
        }
        return textures;
    }

}