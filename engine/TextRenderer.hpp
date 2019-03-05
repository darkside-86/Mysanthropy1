// TextRenderer.hpp
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
#include <unordered_map>

#include <SDL/SDL_ttf.h>

#include "ogl/Texture.hpp"

namespace engine
{

	class TextRenderer
	{
	public:
		TextRenderer();
		virtual ~TextRenderer();
		bool LoadFont(const std::string &path, const std::string& name, int size);
		void UnloadFont(const std::string& name);
		ogl::Texture* RenderText(const std::string& fontName, const std::string& text, bool solid);
		ogl::Texture* RenderTextShaded(const std::string& fontName, const std::string& text);
        void SetFGColor(float r, float g, float b, float a);
        void SetBGColor(float r, float g, float b, float a);
		TTF_Font* GetFont(const std::string & name);
	private:
		TextRenderer(const TextRenderer&) {}
		void operator=(const TextRenderer&) {}

        SDL_Color fg_, bg_;
		std::unordered_map<std::string, TTF_Font*> fonts_;
	};

}