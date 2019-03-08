// ElementBuffer.cpp
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

#include <GL/glew.h>

#include "ElementBuffer.hpp"

namespace ogl
{
    ElementBuffer::ElementBuffer()
    {
        glGenBuffers(1, &id_);
    }
        
    ElementBuffer::~ElementBuffer()
    {
        glDeleteBuffers(1, &id_);
    }

    void ElementBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }
        
    void ElementBuffer::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void ElementBuffer::SetData(long long size, void* data, unsigned int usage)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    }
}