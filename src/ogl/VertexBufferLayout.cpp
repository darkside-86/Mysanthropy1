// VertexBufferLayout.cpp
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

#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include "ErrorCheck.hpp"
#include "VertexBufferLayout.hpp"

namespace ogl
{

	VertexBufferLayout::VertexBufferLayout()
	{
	}

	VertexBufferLayout::~VertexBufferLayout()
	{
	}

	size_t VertexBufferLayout::Element::GetSizeOfType(unsigned int t)
	{
		switch (t)
		{
		case GL_FLOAT:			return sizeof(float);
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:	return sizeof(unsigned char);
		case GL_INT:
		case GL_UNSIGNED_INT:	return sizeof(unsigned int);
		}
		ErrorCheck::PrintErrorf("%s: Unhandled type 0x%x", __FUNCTION__, t);
        return 4; // sane default
	}
}