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

#include <algorithm>

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
		for (auto it = children_.begin(); it != children_.end(); ++it)
		{
			(*it)->parent_ = nullptr;
		}
        if(parent_ != nullptr)
        {
            for(auto it = parent_->children_.begin(); it != parent_->children_.end(); ++it)
            {
                if(this==*it)
                {
                    parent_->children_.erase(it);
                    break;
                }
            }
        }
    }

    void Object::Update(float dtime)
    {
        // check timer callbacks

        //   update elapsed time for each callback and run callback if time has elapsed
        unsigned int ms = (unsigned int)(1000.0f * dtime);
        for(auto it = onTimer_.begin(); it != onTimer_.end(); ++it)
        {
            it->elapsed += ms;
            if(it->elapsed >= it->ms)
            {
                it->callback(TimerEvent());
            }
        }
        //   erase all the ones whose time has run out
        onTimer_.erase(std::remove_if(onTimer_.begin(), onTimer_.end(), [](TimerEventCallbackData data){
            return data.elapsed >= data.ms;
        }), onTimer_.end());

        // finally copy brand new timers into timer list
        onTimer_.insert(onTimer_.end(), newTimers_.begin(), newTimers_.end());
        newTimers_.clear();

        // update children
        for(auto it = children_.begin(); it != children_.end(); ++it)
        {
            (*it)->Update(dtime);
        }
    }

    void Object::AddOnClicked(const ClickedEventCallback& cb)
    {
        onClicked_.push_back(cb);
    }

    void Object::OnClicked(const ClickedEvent& e)
    {
        // if there are no event handlers for this object, propagate event
        //  to the parent
        if(onClicked_.size() == 0)
        {
            if(parent_ != nullptr)
            {
                // parent_->OnClicked(e);
                parent_->OnClicked(ClickedEvent(
                    e.x + this->GetXPos(), 
                    e.y + this->GetYPos(), 
                    e.button));
            }
        }
        else
        {
            for(auto& handler : onClicked_)
                handler(e);
        }
    }

    void Object::AddOnHover(const HoverEventCallback& cb)
    {
        onHover_.push_back(cb);
    }

    void Object::OnHover(const HoverEvent& e)
    {
        // if there are no event handlers for this object, propagate event
        //  to the parent
        if(onHover_.size() == 0)
        {
            if(parent_ != nullptr)
                parent_->OnHover(e);
        }
        else
        {
            for(auto& handler : onHover_)
                handler(e);
        }

    }

    void Object::AddOnKeypressed(const KeypressedEventCallback& cb)
    {
        onKeypressed_.push_back(cb);
    }

    void Object::OnKeypressed(const KeypressedEvent& e)
    {
        // if there are no event handlers for this object, propagate event
        //  to the parent
        if(onKeypressed_.size() == 0)
        {
            if(parent_ != nullptr)
                parent_->OnKeypressed(e);
        }
        else
        {
            for(auto& handler : onKeypressed_)
                handler(e);
        }
    }

    void Object::AddOnDragged(const DraggedEventCallback& cb)
    {
        onDragged_.push_back(cb);
    }
        
    void Object::OnDragged(const DraggedEvent& e)
    {
        // if there are no event handlers for this object, propagate event
        //  to the parent
        if(onDragged_.size() == 0)
        {
            if(parent_ != nullptr)
                parent_->OnDragged(e);
        }
        else
        {
            for(auto& handler : onDragged_)
                handler(e);
        }
    }

    void Object::AddOnTimer(const TimerEventCallback& cb, int ms)
    {
        TimerEventCallbackData data;
        data.callback = cb;
        data.ms = ms;
        // onTimer_.push_back(data);
        newTimers_.push_back(data);
    }

    bool Object::ContainsPoint(int x, int y)
    {
        return (x >= xPos_ && x <= xPos_+width_ &&
            y >= yPos_ && y <= yPos_+height_);
    }

    Object* Object::CheckPoint(int x, int y)
    {
        Object* result = nullptr;
        if(ContainsPoint(x,y) && IsVisible())
        {
            result = this;
            for(int i=(int)children_.size() - 1; i >= 0; --i)
            {
                auto child = children_[i];
                if(child->ContainsPoint(x - xPos_, y - yPos_) && child->IsVisible())
                {
                    result = child->CheckPoint(x - xPos_, y - yPos_);
                    break;
                }
            }
        }
        return result;
    }

}}