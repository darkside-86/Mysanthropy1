// TextureManager.cpp
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

#include "TextureManager.h"

#include "GameEngine.h"

namespace engine
{
    TextureManager::~TextureManager()
    {
        for(auto each : textures_)
        {
            delete each.second;
        }
    }

    void TextureManager::LoadTexture(const std::string& fileName)
    {
        auto found = textures_.find(fileName);
        if(found != textures_.end())
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING, 
                "Texture `%s' already loaded; ignoring.", fileName.c_str());
        }
        else
        {
            ogl::Texture* texture = new ogl::Texture(fileName);
            textures_[fileName] = texture;
        }
    }

    void TextureManager::UnloadTexture(const std::string& fileName)
    {
        auto found = textures_.find(fileName);
        if(found != textures_.end())
        {
            delete found->second;
            textures_.erase(found);
        }
        else
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING, 
                    "Texture `%s' not loaded. Nothing to unload.", fileName.c_str());
        }
    }
    
    ogl::Texture* TextureManager::GetTexture(const std::string& fileName)
    {
        auto found = textures_.find(fileName);
        if(found != textures_.end())
        {
            return found->second;
        }
        else
        {
            // load the file first then return it
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING, 
                    "Texture `%s' not preloaded... Attempting to load.", fileName.c_str());
            LoadTexture(fileName);
            found = textures_.find(fileName);
            if(found != textures_.end())
                return found->second;
            return nullptr;
        }
    }
}