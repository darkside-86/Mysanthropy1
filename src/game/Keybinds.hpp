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

// Defines the type of function to be called when a key is pressed
typedef std::function<void(void)> Keybind;

// Associates functions with keycodes enumerated by SDL
class Keybinds
{
public:
    // ctor
    Keybinds();
    // dtor
    virtual ~Keybinds();   
    // TODO: Rename this to set because only one keybind callback per keycode
    void AddKeybind(int keycode, const Keybind& kb);
    // Runs the function (if any) associated with a given SDLK_* keycode
    void RunKeybind(int keycode);
    // Erase all existing callbacks
    void Clear();
private:
    // Table of keycode-function pairs
    std::unordered_map<int, Keybind> keybindCallbacks_;
};