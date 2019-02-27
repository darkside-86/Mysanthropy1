// Object.h
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

#include <vector>

#include <SDL/SDL.h>

#include "../GraphicsContext.h"

#include "Event.h"

namespace engine { namespace ui {

    // Defines the base abstract class for all UI objects.
    class Object
    {
    public: 
        // Constructor. All UI objects must have a parent object except for ui::Root
        Object(Object* parent);
        // Destructor. 
        virtual ~Object();
        // Adds an event handler to be called when the object is clicked.
        virtual void AddOnClicked(const ClickedEventCallback& cb);
        // Runs the list of click event handlers.
        virtual void OnClicked(const ClickedEvent& e);
        // Adds an event handler to be called when object is hovered over.
        virtual void AddOnHover(const HoverEventCallback& cb);
        // Runs the list of hover event handlers
        virtual void OnHover(const HoverEvent& e);
        // Adds an event handler to be called when the object has focus and a key is pressed.
        virtual void AddOnKeypressed(const KeypressedEventCallback& cb);
        // Runs the list of keyboard press event handlers
        virtual void OnKeypressed(const KeypressedEvent& e);
        // Adds an event handler for when the mouse is pressed, then moved, before release
        virtual void AddOnDragged(const DraggedEventCallback& cb);
        // Runs the list of event handlers for when the object is "dragged"
        virtual void OnDragged(const DraggedEvent& e);
        // Adds a function to be called after specified number of milliseconds.
        virtual void AddOnTimer(const TimerEventCallback& cb, int ms);
        // Check if a logical screen coordinate is within the object
        bool ContainsPoint(int x, int y);
        // get the innermost child object that contains the point if any
        Object* CheckPoint(int x, int y);
        // Returns the parent object
        inline Object* GetParent() { return parent_; }
        // Returns true if object is visible. Otherwise false.
        inline bool IsVisible() const {return visible_;}
        // Sets the visibility of the object.
        inline void SetVisible(bool visible) { visible_ = visible; }
        // Each subclass of Object renders differently. Because this method is
        // abstract, each subclass's implementation of Render must manually check visibility
        virtual void Render(GraphicsContext& gc) = 0;
        // Updates any timers associated with object
        virtual void Update(float dtime);
        // Returns the width of the object in logical pixels
        inline virtual int GetWidth() const { return width_; }
        // Returns the height of the object in logical pixels
        inline virtual int GetHeight() const { return height_; }
        // Sets the width of the object. Virtual because each subclass has to
        // recreate opengl objects to modify the width.
        inline virtual void SetWidth(int w) { width_ = w; }
        // Sets the height of the object. See SetWidth
        inline virtual void SetHeight(int h) { height_ = h; }
        // Returns the x position of the object, in logical pixels relative to the parent object
        inline virtual int GetXPos() const { return xPos_; }
        // Returns the y position of the object, in logical pixels relative to the parent object
        inline virtual int GetYPos() const { return yPos_; }
        // Sets the x position of the object in logical pixels relative to the parent object
        inline virtual void SetXPos(int x) { xPos_ = x; }
        // Sets the y position of the object in logical pixels relative to the parent object
        inline virtual void SetYPos(int y) { yPos_ = y; }
    protected:
        // Children of this object. Their positions are always relative to the parent position
        std::vector<Object*>                children_;
        // The list of click event handlers
        std::vector<ClickedEventCallback>   onClicked_;
        // The list of hover event handlers
        std::vector<HoverEventCallback>     onHover_;
        // The list of key press event handlers
        std::vector<KeypressedEventCallback> onKeypressed_;
        // The list of "drag" event handlers
        std::vector<DraggedEventCallback>   onDragged_;
        // The list of timers
        std::vector<TimerEventCallbackData> onTimer_;
        // Anytime a timer is added, it is placed here first so that timer callbacks can add new
        // timers to the same object safely. These timers subsequently get added to the onTimer_ list
        std::vector<TimerEventCallbackData> newTimers_;
        // The parent UI object
        Object*                             parent_;
        // Whether or not the object should be rendered. Each subclass must check this in Render manually
        bool                                visible_ = true;
        // The width and height of the object in logical screen pixel coordinates.
        int                                 width_=1, height_=1;
        // The position in logical pixel coordinates. Relative to parent_. If parent_ is null or located
        // at 0,0, then these are the absolute screen coordinates.
        int                                 xPos_=0, yPos_=0;
    };

}}