// VertexArray.hpp
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

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

namespace ogl
{
	// Encapsulates the OpenGL VAOs.
    class VertexArray
    {
    public:
		VertexArray();
		~VertexArray();
		// Associates raw buffer with this vao
		//  vb - the raw buffer
		//  layout - The metadata about how the data is used.
		void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout& layout);
		void Bind() const;
		void Unbind() const;
	private:
		unsigned int id_ = 0;
    };
}