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
#include <unordered_map>

#include <SDL/SDL_mixer.h>

namespace engine
{
    class SoundManager
    {
    public:
        SoundManager();
        ~SoundManager();
        void LoadSound(const std::string& path);
        // returns what channel the sound will be played on
        int PlaySound(const std::string& path);
        void UnloadSound(const std::string& path);
        // stop a channel as returned by PlaySound
        void HaltSound(int channel);
        void PlayMusic(const std::string& path, int loops);
        void StopMusic();
    private:
        // one game engine means one sound manager. this setup is required because
        //  C++ doesn't support delegates as function pointers.
        static SoundManager& singleton_;
        std::unordered_map<std::string, Mix_Chunk*> sounds_;
        Mix_Music* music_ = nullptr;
    };
}