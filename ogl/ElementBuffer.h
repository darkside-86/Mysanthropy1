// ElementBuffer.h
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

namespace ogl
{
    // Encapsulates an index buffer. Should be bound before calling
    //  VertexArray::SetBuffer() in order to associate it with the
    //  vao.
    class ElementBuffer
    {
    public:
        ElementBuffer();
        ~ElementBuffer();
        void Bind();
        void Unbind();
        // Sets a raw array of numbers as the data. The type of number
        //  is specified in glDrawElements calls so this isn't hardcoded here.
        void SetData(long long size, void* data, unsigned int usage);
    private:
        ElementBuffer(const ElementBuffer&) {}
        void operator=(const ElementBuffer&) {}
        unsigned int id_;
    };
}