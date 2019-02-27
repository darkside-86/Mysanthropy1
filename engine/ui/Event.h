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

    // Base class for all events. Contains rudimentary RTTI that is unused so far
    class Event 
    {
    public:
        enum class Type {
            CLICKED, HOVER, KEYPRESSED, DRAGGED, TIMER
        };
        Event(Type t) : type(t) {}
        Type type;
    };

    // Defines an event parameter for when a UI object is clicked. To be clicked means
    // that the mouse button is down while the cursor is over the object, followed by a release
    // of the mouse button while the cursor is still over the object.
    class ClickedEvent : Event 
    {
    public:
        ClickedEvent(int sx, int sy, int btn) : Event(Type::CLICKED),
            x(sx), y(sy), button(btn) {}
        int x, y, button;
    };

    // Defines an event parameter for when the mouse cursor either moves into the area of the
    // UI object, or when the mouse moves while the cursor is still within the bounds of the
    // object. Hover events are also generated when the mouse cursor leaves the bounds of the
    // object. In such a case, the "over" member is false.
    class HoverEvent : Event
    {
    public:
        HoverEvent(int sx, int sy, int dx, int dy, bool o) : Event(Type::HOVER),
            x(sx), y(sy), xrel(dx), yrel(dy), over(o) {}
        // The relative logical pixel coordinates of the mouse based on UI parent tree.
        int x, y; 
        // The amount of logical pixel movement
        int xrel, yrel;
        // false when mouse moves away from object. True any other time event is fired
        bool over; 
    };

    // Defines an event parameter for when a keyboard button is pressed while the UI object has
    // "focus." Focus occurs when the object was previously clicked and no other object has been clicked.
    class KeypressedEvent : Event 
    {
    public:
        KeypressedEvent(int keycode, int scancode, int modifiers, bool repeat) : Event(Type::KEYPRESSED),
            keyCode(keycode), scanCode(scancode), mod(modifiers), repeated(repeat) {}
        // The SDL keycode e.g. SDLK_UP, the sdl scancode, and key modifier flags
        int keyCode, scanCode, mod;
        // True if the event was fired as a result of the user holding down on the same key without releasing.
        // otherwise false.
        bool repeated;
    };

    // Defines an event parameter for when a UI object is "dragged" with the mouse. Dragging is when
    // the mouse button is pressed down over the object, and the mouse is moved before the mouse button
    // is released.
    class DraggedEvent : Event
    {
    public:
        DraggedEvent(int sx, int sy, int dx, int dy) : Event(Type::DRAGGED),
            x(sx), y(sy), xrel(dx), yrel(dy) {}
        // the final location of the mouse
        int x, y;
        // the direction the object was "dragged." Object does not necessarily move.
        int xrel, yrel;
    };

    // Defines an event parameter with no information so far for when a "timer" callback is run.
    class TimerEvent : Event 
    {
    public:
        TimerEvent() : Event(Type::TIMER) {}
    };

    // Defines a type of callback to handle when an object is clicked.
    typedef std::function<void(const ClickedEvent&)> ClickedEventCallback;
    // Defines a type of callback to handle when an object is hovered over.
    typedef std::function<void(const HoverEvent&)> HoverEventCallback;
    // Defines a type of callback to handle when a focused object receives a keystroke
    typedef std::function<void(const KeypressedEvent&)> KeypressedEventCallback;
    // Defines a type of callback to handle when an object is "dragged"
    typedef std::function<void(const DraggedEvent&)> DraggedEventCallback;
    // Defines a callback to be run when a timer has expired. TimerEventCallbacks can safely
    // add new TimerCallbacks to the same object to create repeating ticking callbacks.
    typedef std::function<void(const TimerEvent&)> TimerEventCallback;

    // Defines a data structure to keep track of when a timer event callback should be run then discarded.
    struct TimerEventCallbackData
    {
        TimerEventCallback callback;
        unsigned int ms;
        unsigned int elapsed = 0;
    };
}}