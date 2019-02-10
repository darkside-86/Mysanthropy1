// Mesh.h
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
#pragma once

#include <vector>

#include "ElementBuffer.h"
#include "Program.h"
#include "Texture.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace ogl
{
    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, 
             const std::vector<unsigned int>& indices, 
             const std::vector<Texture*>& textures);
        virtual ~Mesh();
        void Draw(Program& program);
        void DestroyTextures();
    private:
        void SetupMesh();
        std::vector<Vertex> vertices_;
        std::vector<unsigned int> indices_;
        std::vector<Texture*> textures_;
        VertexArray vao_;
        VertexBuffer vbo_;
        ElementBuffer ebo_;
    };
}