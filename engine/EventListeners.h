// EventListeners.h
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

#include <functional>

#include <SDL/SDL.h>

namespace engine
{
    // defines a generic function signature for handling any SDL_Event
    typedef std::function<void(const SDL_Event&)> EventListener;
    // defines a function signature for handling SDL_Keyboard events
    typedef std::function<void(const SDL_KeyboardEvent&)> KeyboardListener;
    // defines a function signature for handling SDL_MouseButtonEvents
    typedef std::function<void(const SDL_MouseButtonEvent&)> MouseButtonListener;
    // defines a function signature for handling SDL_MouseMotionEvents
    typedef std::function<void(const SDL_MouseMotionEvent&)> MouseMotionListener;
}
