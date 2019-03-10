// Keybinds.hpp
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

#include <functional>
#include <unordered_map>

#include <SDL/SDL_keycode.h>

typedef std::function<void(void)> Keybind;

class Keybinds
{
public:
    Keybinds();
    virtual ~Keybinds();

    void AddKeybind(int keycode, const Keybind& kb);
    void RunKeybind(int keycode);
    void Clear();
private:
    std::unordered_map<int, Keybind> keybindCallbacks_;
};