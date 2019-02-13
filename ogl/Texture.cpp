// Texture.cpp 
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

#include "Texture.h"

#include <iostream>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "ErrorCheck.h"

namespace ogl
{

	Texture::Texture(const std::string& filePath, bool linear, bool repeat, Texture::TYPE type) : type_(type)
	{
		// load and convert SDL surface to RGBA (32-bits)
		//
		SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
		if (loadedSurface == nullptr)
		{
			ErrorCheck::PrintErrorf("%s: Error loading `%s' -- %s", __FUNCTION__, 
									filePath.c_str(), IMG_GetError());
			return;
		}
		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA32, 0);
		SDL_FreeSurface(loadedSurface);

		if (formattedSurface == nullptr)
		{
			ErrorCheck::PrintErrorf("%s: Error formatting image -- %s", __FUNCTION__, SDL_GetError());
			return;
		}

		// generate texture ID and bind it
		//
		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_2D, id_);

		// set linear vs. nearest
		//
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		// TODO: clamp vs repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, formattedSurface->w, formattedSurface->h,
			0, GL_RGBA, GL_UNSIGNED_BYTE, formattedSurface->pixels);
		glActiveTexture(GL_TEXTURE0); // place in slot 0
		OGL_ERROR_CHECK();

		width_ = formattedSurface->w;
		height_ = formattedSurface->h;

		SDL_FreeSurface(formattedSurface); // throw away data on CPU side
	}

	Texture::Texture(const void* rgbaBuffer, int w, int h, bool linear, bool repeat, Texture::TYPE type) 
		: type_(type)
	{
		// generate texture ID and bind it
		//
		glActiveTexture(GL_TEXTURE0); // place in slot 0
		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_2D, id_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		// TODO: clamp vs repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h,
			0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaBuffer);
		width_ = w;
		height_ = h;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &id_);
	}

	void Texture::Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id_);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::SetRepeat(bool repeat)
	{
		glBindTexture(GL_TEXTURE_2D, id_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat? GL_REPEAT : GL_CLAMP_TO_EDGE);
	}

}