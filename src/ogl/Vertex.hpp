// Vertex.hpp
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

#include "VertexBufferLayout.hpp"

namespace ogl
{
    struct Vertex
	{
		struct Position
		{
			float x;
			float y;
			float z;
		}
		position;

		struct Color
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		}
		color;

		struct TextureCoords
		{
			float s;
			float t;
		}
		texCoords;

		struct Normal 
		{
			float x;
			float y;
			float z;
		}
		normal;

		static void PushLayout(VertexBufferLayout& vbl);
    };
}