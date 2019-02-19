// SoundManager.cpp
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

#include <string>

#include <SDL/SDL_mixer.h>

namespace engine
{
    class SoundManager
    {
    public:
        SoundManager();
        ~SoundManager();
        void PlaySound(const std::string& path);
        void PlayMusic(const std::string& path);
        void StopMusic();
    private:
        Mix_Music* music_ = nullptr;
    };
}