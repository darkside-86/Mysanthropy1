// Texture.hpp
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

#include <string>

namespace ogl
{
	// Encapsulates OpenGL textures
	class Texture
	{
	public:
		enum TYPE { DIFFUSE, SPECULAR };
		Texture(const std::string& filePath, bool linear = true, bool repeat=false, TYPE type=DIFFUSE);
		Texture(int w, int h,const void* rgbaBuffer, bool linear=true, bool repeat=false, TYPE type=DIFFUSE);
		virtual ~Texture();
		// Binds the texture for usage. Slots can be specified with GL_TEXTURE0...
		//  A uniform of type sampler2D should be set to the slot value to be used
		//  by shaders.
		// slot - GL_TEXTURE0 by default. Check platform for available number of slots.
		void Bind(unsigned int slot = 0) const;
		void Unbind() const;
		// Returns the image width in pixels
		inline unsigned int GetWidth() const { return width_; }
		// Returns the image height in pixels
		inline unsigned int GetHeight() const { return height_; }
		// get type
		inline TYPE GetType () { return type_; }
		// get path. Note: this will be "" if not loaded from file
		inline std::string GetPath() const { return path_; }
		// set clamp vs repeat
		void SetRepeat(bool repeat);
		// SubImage - accepts RGBA data with an unsigned byte per component.
		void SubImage(int xoffset, int yoffset, int subWidth, int subHeight, void* data);
	private:
		Texture(const Texture&) {}
		void operator=(const Texture&) {}
		unsigned int id_ = 0;
		unsigned int width_ = 0;
		unsigned int height_ = 0;
		std::string path_ = ""; // remains "" unless texture is created with constructor using path
		TYPE type_;
	};
}

