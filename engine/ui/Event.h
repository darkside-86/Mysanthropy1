// Event.h
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

namespace engine { namespace ui {

    class Event 
    {
    public:
        enum class Type {
            CLICKED, HOVER
        };
        Event(Type t) : type(t) {}
        Type type;
    };

    class ClickedEvent : Event 
    {
    public:
        ClickedEvent(int sx, int sy, int btn) : Event(Type::CLICKED),
            x(sx), y(sy), button(btn) {}
        int x, y, button;
    };

    class HoverEvent : Event
    {
    public:
        HoverEvent(int sx, int sy, int dx, int dy, bool o) : Event(Type::HOVER),
            x(sx), y(sy), xrel(dx), yrel(dy), over(o) {}
        int x, y, xrel, yrel;
        bool over; // false when mouse moves away
    };

    typedef std::function<void(const ClickedEvent&)> ClickedEventCallback;
    typedef std::function<void(const HoverEvent&)> HoverEventCallback;
}}