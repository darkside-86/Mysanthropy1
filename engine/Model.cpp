#include "Model.h"

#include "engine/GameEngine.h"

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
        // process materials
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<ogl::Texture*> diffuseMaps = LoadMaterialTextures(material, 
                    aiTextureType_DIFFUSE, ogl::Texture::TYPE::DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<ogl::Texture*> specularMaps = LoadMaterialTextures(material,
                    aiTextureType_SPECULAR, ogl::Texture::TYPE::SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return new Mesh(vertices, indices, textures);
    }

    std::vector<ogl::Texture*> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, ogl::Texture::TYPE t)
    {
        std::vector<ogl::Texture*> textures;
        for(unsigned int i=0; i < mat->GetTextureCount(type); ++i)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            ogl::Texture* texture = new ogl::Texture(directory_ + "/" + std::string(str.C_Str()), true, t);
            textures.push_back(texture);
        }
        return textures;
    }

}