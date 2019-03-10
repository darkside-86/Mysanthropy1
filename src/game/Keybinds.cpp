// Keybinds.cpp
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

#include "Keybinds.hpp"

Keybinds::Keybinds()
{

}

Keybinds::~Keybinds()
{

}

void Keybinds::AddKeybind(int keycode, const Keybind& kb)
{
    keybindCallbacks_[keycode] = kb;
}

void Keybinds::RunKeybind(int keycode)
{
    auto& found = keybindCallbacks_.find(keycode);
    if(found != keybindCallbacks_.end())
    {
        found->second();
    }
}

void Keybinds::Clear()
{
    keybindCallbacks_.clear();
}