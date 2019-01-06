// TextRenderer.cpp
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

#include "TextRenderer.h"

#include "GameEngine.h"

namespace engine
{
    TextRenderer::TextRenderer()
    {
        // black text on white background by default
        fg_.r = fg_.b = fg_.g = 0;
        fg_.a = 255;
        bg_.r = bg_.b = bg_.g = bg_.a = 255;
    }
	
    TextRenderer::~TextRenderer()
    {
		for (auto eachFont : fonts_)
		{
			TTF_CloseFont(eachFont.second);
		}
    }

	bool TextRenderer::LoadFont(const std::string &path, const std::string& name, int size)
    {
        // no overwriting existing fonts
		auto found = fonts_.find(name);
		if (found != fonts_.end())
		{
			GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING, 
                    "%s: Font `%s' already loaded!", __FUNCTION__, name.c_str());
			return true;
		}

		TTF_Font* font = TTF_OpenFont(path.c_str(), size);
		if (font == nullptr)
		{
			GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING,
                    "%s: %s", __FUNCTION__, TTF_GetError());
			return false;
		}

	    fonts_[name] = font;

		return true;
    }

	ogl::Texture* TextRenderer::RenderText(const std::string& fontName, const std::string& text, bool solid)
    {
		SDL_Surface* rendered;
		TTF_Font* font = GetFont(fontName);
		if (font == nullptr)
			return nullptr;
        rendered = solid? TTF_RenderText_Solid(font, text.c_str(), fg_) :
                          TTF_RenderText_Blended(font, text.c_str(), fg_);
		if (rendered == nullptr)
		{
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING,
                    "%s: %s", __FUNCTION__, TTF_GetError());
			return nullptr;
		}
		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(rendered, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(rendered);
        if(formattedSurface == nullptr)
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING,
                    "%s: %s", __FUNCTION__, SDL_GetError());
			return nullptr;
        }
		// hard hack for alpha blend because TTF implementation fails to do it
		SDL_Color* pixels = (SDL_Color*)formattedSurface->pixels;
		unsigned char alpha = fg_.a;
		for (int i = 0; i < formattedSurface->w * formattedSurface->h; ++i)
		{
			pixels[i].a &= alpha;
        }
		ogl::Texture* texture = new ogl::Texture(pixels, formattedSurface->w, 
                formattedSurface->h, !solid);
		SDL_FreeSurface(formattedSurface);

		return texture;
    }

	ogl::Texture* TextRenderer::RenderTextShaded(const std::string& fontName, const std::string& text)
    {
		SDL_Surface* rendered;
		TTF_Font* font = GetFont(fontName);
		if (font == nullptr)
			return nullptr;
		rendered = TTF_RenderText_Shaded(font, text.c_str(), fg_, bg_);
		if (rendered == nullptr)
		{
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING,
                    "%s: %s", __FUNCTION__, TTF_GetError());
			return nullptr;
		}
		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(rendered, SDL_PIXELFORMAT_RGBA32, 0);
		SDL_FreeSurface(rendered);
        if(formattedSurface == nullptr)
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING,
                    "%s: %s", __FUNCTION__, SDL_GetError());
			return nullptr;
        }
		ogl::Texture* texture = new ogl::Texture(formattedSurface->pixels, formattedSurface->w, 
                formattedSurface->h, false);
		SDL_FreeSurface(formattedSurface);

        return texture;
    }

    void TextRenderer::SetFGColor(float r, float g, float b, float a)
    {
        fg_.r = (unsigned char)(255.f * r);
        fg_.g = (unsigned char)(255.f * g);
        fg_.b = (unsigned char)(255.f * b);
        fg_.a = (unsigned char)(255.f * a);
    }

    void TextRenderer::SetBGColor(float r, float g, float b, float a)
    {
        bg_.r = (unsigned char)(255.f * r);
        bg_.g = (unsigned char)(255.f * g);
        bg_.b = (unsigned char)(255.f * b);
        bg_.a = (unsigned char)(255.f * a);
    }

    TTF_Font* TextRenderer::GetFont(const std::string & name)
    {
		auto findFont = fonts_.find(name);
		if (findFont == fonts_.end())
		{
			GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING,
                    "%s: Font `%s' does not exist", __FUNCTION__, name.c_str());
			return nullptr;
		}
		return findFont->second;
    }
}