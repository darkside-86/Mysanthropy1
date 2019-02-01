// Slider.cpp
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

#include "Slider.h"

namespace engine { namespace  ui {

    Slider::Slider(Object* parent, int width, int height, ORIENTATION orientation, ogl::Texture* texture) :
        Frame(parent, width, height, 0,0, texture, {0,0,0,0}), orientation_(orientation)
    {
        constexpr Color SLIDE_COLOR = {0.2f, 0.2f, 0.2f, 1.f};
        constexpr Color KNOB_COLOR = {0.7f, 0.7f, 0.7f, 1.f};
        if(orientation_ == ORIENTATION::HORIZONTAL)
        {
            slide_ = new Frame(this, width, height / 10, 0, 0, texture, SLIDE_COLOR);
            slide_->SetYPos(GetHeight() / 2 - slide_->GetHeight() / 2);
            knob_ = new Frame(this, width / 25, height, 0, 0, texture, KNOB_COLOR);
            AddOnDragged([this](const DraggedEvent& e) {
                knob_->SetXPos(knob_->GetXPos() + e.xrel);
                if(knob_->GetXPos() < 0)
                    knob_->SetXPos(0);
                else if(knob_->GetXPos() + knob_->GetWidth() > GetWidth())
                    knob_->SetXPos(GetWidth() - knob_->GetWidth());
            });
        }
        else
        {
            slide_ = new Frame(this, width / 10, height, 0, 0, texture, SLIDE_COLOR);
            slide_->SetXPos(GetWidth() / 2 - slide_->GetWidth() / 2);
            knob_ = new Frame(this, width, height / 25, 0, 0, texture, KNOB_COLOR);
            knob_->SetYPos(GetHeight() - knob_->GetHeight()); // value=0 position
            AddOnDragged([this](const DraggedEvent& e) {
                knob_->SetYPos(knob_->GetYPos() + e.yrel);
                if(knob_->GetYPos() < 0)
                    knob_->SetYPos(0);
                else if(knob_->GetYPos() + knob_->GetHeight() > GetHeight())
                    knob_->SetYPos(GetHeight() - knob_->GetHeight());
            });
        }
        
    }

    Slider::~Slider()
    {
        delete knob_;
        delete slide_;
    }

    void Slider::SetSlideColor(const Color& c)
    {
        slide_->SetColor(c);
    }

    Color Slider::GetSlideColor()
    {
        return slide_->GetColor();
    }

    void Slider::SetKnobColor(const Color& c)
    {
        knob_->SetColor(c);
    }

    Color Slider::GetKnobColor()
    {
        return knob_->GetColor();
    }

    void Slider::SetValue(double value)
    {
        if(orientation_ == ORIENTATION::HORIZONTAL)
        {
            knob_->SetXPos((int)(value * (double)(GetWidth() - knob_->GetWidth())));
        }
        else
        {
            knob_->SetYPos((int)((1.0 - value) * (double)(GetHeight() - knob_->GetHeight())));
        }
    }
    
    double Slider::GetValue()
    {
        double value;
        if(orientation_ == ORIENTATION::HORIZONTAL)
        {
            value = (double)(knob_->GetXPos()) / ((double)(GetWidth() - knob_->GetWidth()));
        }
        else 
        {
            value = 1.0 - (double)(knob_->GetYPos()) / (double)((GetHeight() - knob_->GetHeight()));
        }
        return value;
    }

}}
