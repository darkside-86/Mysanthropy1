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
#include "ErrorCheck.h"

namespace ogl
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, 
               const std::vector<unsigned int>& indices, 
               const std::vector<Texture*>& textures)
    {
        vertices_ = vertices;
        indices_ = indices;
        textures_ = textures;
        SetupMesh();
    }

    Mesh::~Mesh() 
    {

    }

    void Mesh::Draw(Program& program)
    {
        int diffuseCounter = 0;
        int specularCounter = 0;

        for(int i=0; i < textures_.size(); ++i)
        {
            std::string typeStr, indexStr;
            if(textures_[i]->GetType() == Texture::TYPE::DIFFUSE)
            {
                typeStr = "diffuse";
                indexStr = std::string("[") + std::to_string(diffuseCounter++) + "]";
            }
            else   
            {
                typeStr = "specular";
                indexStr = std::string("[") + std::to_string(specularCounter++) + "]";
            }
            // std::string textureName = std::string("texture_") + typeStr + indexStr;
            std::string materialName = std::string("u_material") + indexStr + "." + typeStr;
            // program.SetUniform<int>(textureName, i);
            program.SetUniform<int>(materialName, i);
            textures_[i]->Bind(i);
        }

        vao_.Bind();
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