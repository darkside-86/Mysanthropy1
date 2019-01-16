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

    class Object
    {
    public: 
        Object(Object* parent);
        virtual ~Object();
        virtual void AddOnClicked(const ClickedEventCallback& cb);
        virtual void OnClicked(const ClickedEvent& e);
        virtual void AddOnHover(const HoverEventCallback& cb);
        virtual void OnHover(const HoverEvent& e);
        bool ContainsPoint(int x, int y);
        // get the innermost child object that contains the point if any
        Object* CheckPoint(int x, int y);
        inline bool IsVisible() {return visible_;}
        inline void SetVisible(bool visible) { visible_ = visible; }
        virtual void Render(GraphicsContext& gc) = 0;
        virtual int GetWidth() { return width_; }
        virtual int GetHeight() { return height_; }
        virtual void SetWidth(int w) { width_ = w; }
        virtual void SetHeight(int h) { height_ = h; }
        virtual int GetXPos() { return xPos_; }
        virtual int GetYPos() { return yPos_; }
        virtual void SetXPos(int x) { xPos_ = x; }
        virtual void SetYPos(int y) { yPos_ = y; }

    protected:
        std::vector<Object*>                children_;
        std::vector<ClickedEventCallback>   onClicked_;
        std::vector<HoverEventCallback>     onHover_;
        Object*                             parent_;
        bool                                visible_;
        int                                 width_=1, height_=1;
        int                                 xPos_=0, yPos_=0;
    };

}}