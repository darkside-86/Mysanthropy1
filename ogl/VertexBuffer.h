// VertexBuffer.h
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
    // Encapsulates the raw data of an OpenGL general buffer
    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();
        void Bind() const;
        void Unbind() const;
        // Sets the initial raw data
        void SetData(long long size, void* data, unsigned int usage);
    private:
        VertexBuffer(const VertexBuffer&) {}
        void operator=(const VertexBuffer&) {}

        unsigned int id_;
    };
}