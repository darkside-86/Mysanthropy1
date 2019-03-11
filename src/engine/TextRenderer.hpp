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
		// ctor
		TextRenderer();
		// dtor
		virtual ~TextRenderer();
		// Loads and stores a font in the table, accessible by an alias
		bool LoadFont(const std::string &path, const std::string& name, int size);
		// Unloads a font stored under an alias
		void UnloadFont(const std::string& name);
		// Renders text with given font by alias as a Texture. CALLER OWNS THE POINTER!
		ogl::Texture* RenderText(const std::string& fontName, const std::string& text, bool solid);
		// Renders shaded (has background color) text. CALLER OWNS THE POINTER!
		ogl::Texture* RenderTextShaded(const std::string& fontName, const std::string& text);
		// Sets the foreground color of text, affecting only RenderText
        void SetFGColor(float r, float g, float b, float a);
		// Sets the background color of "shaded" text rendered
        void SetBGColor(float r, float g, float b, float a);
		// Deprecated. Returns a TTF_Font associated with an alias. Caller does not own pointer.
		TTF_Font* GetFont(const std::string & name);
		// TODO: void GetTextSize(int& w, int& h) for getting dimensions without rendering the text
	private:
		// Copy constructor is disabled
		TextRenderer(const TextRenderer&) {}
		// See above
		void operator=(const TextRenderer&) {}
		// foreground and background colors
        SDL_Color fg_, bg_;
		// the list of fonts accessed by alias
		std::unordered_map<std::string, TTF_Font*> fonts_;
	};

}