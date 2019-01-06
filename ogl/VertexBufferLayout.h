// VertexBufferLayout.h
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

#include <GL/glew.h>

namespace ogl
{
	// Encapsulates vertex attributes
    class VertexBufferLayout
    {
    public:
		struct Element
		{
			unsigned int type;
			unsigned int count;
			bool normalized;
			static size_t GetSizeOfType(unsigned int t);
		};

		VertexBufferLayout();
		~VertexBufferLayout();

		// Pushes an attribute specified by a type and count
		//  count - Number of type T
		//  normalized - if true, openGL will set the value to between 0 and 1
		template<typename T>
		void Push(unsigned int count, bool normalized) { static_assert(false); } // not handled

		template<>
		void Push<float>(unsigned int count, bool normalized)
		{
			elements_.push_back({ GL_FLOAT, count, normalized });
			stride_ += sizeof(float) * count;
		}

		template<>
		void Push<unsigned char>(unsigned int count, bool normalized)
		{
			elements_.push_back({ GL_UNSIGNED_BYTE, count, normalized });
			stride_ += sizeof(unsigned char) * count;
		}

		// TODO: more data types

		inline const std::vector<VertexBufferLayout::Element>& GetElements() const { return elements_; }
		inline unsigned int GetStride() const { return stride_; }

	private:
		std::vector<VertexBufferLayout::Element> elements_;
		unsigned int stride_ = 0;
	};
}