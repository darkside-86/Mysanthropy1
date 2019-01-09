// Object.cpp
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

#include "Object.h"

namespace engine { namespace ui {

    Object::Object(Object* parent) : parent_(parent), visible_(true)
    {
        if(parent_ != nullptr)
        {
            parent_->children_.push_back(this);
        }
    }

    Object::~Object()
    {
        /* for(auto eachChild : children_)
            delete eachChild; */
    }

    void Object::AddOnClicked(const ClickedEventCallback& cb)
    {
        onClicked_.push_back(cb);
    }

    void Object::OnClicked(int x, int y, int btn)
    {
        for(auto& handler : onClicked_)
            handler(ClickedEvent(x, y, btn));
    }

    bool Object::ContainsPoint(int x, int y)
    {
        return (x >= xPos_ && x <= xPos_+width_ &&
            y >= yPos_ && y <= yPos_+height_);
    }

    Object* Object::CheckPoint(int x, int y)
    {
        Object* result = nullptr;
        if(ContainsPoint(x,y))
        {
            result = this;
            for(auto child : children_)
            {
                if(ContainsPoint(x - xPos_, y - yPos_))
                {
                    result = child->CheckPoint(x - xPos_, y - yPos_);
                    break;
                }
            }
        }
        return result;
    }

}}