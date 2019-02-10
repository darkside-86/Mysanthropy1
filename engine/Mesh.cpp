// Mesh.cpp
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2018
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

#include "Mesh.h"
#include "ogl/ErrorCheck.h"

namespace engine
{
    using namespace ogl;

    Mesh::Mesh(const std::vector<ogl::Vertex>& vertices, 
               const std::vector<unsigned int>& indices, 
               const std::vector<ogl::Texture*>& textures, 
               const Material& material)
    {
        vertices_ = vertices;
        indices_ = indices;
        textures_ = textures;
        material_ = material;
        SetupMesh();
    }

    Mesh::~Mesh() 
    {

    }

    void Mesh::Draw(Program& program)
    {
        int diffuseCounter = 1;
        int specularCounter = 1;

        for(int i=0; i < textures_.size(); ++i)
        {
            std::string typeStr, indexStr;
            if(textures_[i]->GetType() == Texture::TYPE::DIFFUSE)
            {
                typeStr = "texture_diffuse";
                indexStr = std::to_string(diffuseCounter++);
            }
            else   
            {
                typeStr = "texture_specular";
                indexStr = std::to_string(specularCounter++);
            }
            // std::string textureName = std::string("texture_") + typeStr + indexStr;
            std::string uniformName = typeStr + indexStr;
            // program.SetUniform<int>(textureName, i);
            OGL_ERROR_CHECK();
            program.SetUniform<int>(uniformName, i);
            OGL_ERROR_CHECK();
            textures_[i]->Bind(i);
        }

        // set material properties
        program.SetUniform<glm::vec3>("u_material.diffuse", material_.diffuse);
        program.SetUniform<glm::vec3>("u_material.specular", material_.specular);
        program.SetUniform<glm::vec3>("u_material.ambient", material_.ambient);
        program.SetUniform<float>("u_material.shininess", material_.shininess);

        vao_.Bind();
        OGL_ERROR_CHECK();
        glDrawElements(GL_TRIANGLES, (GLsizei)indices_.size(), GL_UNSIGNED_INT, nullptr);
        OGL_ERROR_CHECK();
        vao_.Unbind();
        glActiveTexture(GL_TEXTURE0);
    }

    void Mesh::DestroyTextures()
    {
        for(auto texture : textures_)
            delete texture;
        textures_.clear();
    }

    void Mesh::SetupMesh()
    {
        vbo_.SetData(vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);
        ebo_.SetData(indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);
        VertexBufferLayout vbl;
        Vertex::PushLayout(vbl);
        vao_.AddBuffer(vbo_, vbl);
    }
}