// VertexArray.cpp
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
#include "VertexArray.h"

#include <GL/glew.h>

namespace ogl
{

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &id_);
        glBindVertexArray(id_); // this seems to be required 
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &id_);
	}

	void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout)
	{
        glBindVertexArray(id_);
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); ++i)
		{
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE,
								  layout.GetStride(), (void*)offset);
			offset += element.count * static_cast<unsigned int>(VertexBufferLayout::Element::GetSizeOfType(element.type));
		}
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(id_);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

}