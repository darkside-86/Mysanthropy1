// Button.cpp
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

#include "Button.hpp"

#include "../GameEngine.hpp"

namespace engine { namespace ui {

    Button::Button(Object* parent, ogl::Texture* texture, const Color& color, const std::string& text, 
                   const std::string fontAlias, const Color& textColor, int padding) 
                : Frame(parent,0,0,0,0,texture, color), text_(text), font_(fontAlias),
                    labelColor_(textColor), padding_(padding)
    {
        CreateLabel();
    }

    Button::~Button()
    {
        delete labelVao_;
        delete labelVbo_;
        delete labelTexture_;
    }

    void Button::Render(GraphicsContext& gc)
    {
        if(!IsVisible())
            return;
        Frame::Render(gc);
        gc.PushModel();
        gc.TranslateModel((float)(xPos_+padding_), (float)(yPos_+padding_), 0.f);
        gc.SetMVP();
        if(labelVao_ != nullptr) labelVao_->Bind();
        if(labelVbo_ != nullptr) labelVbo_->Bind();
        if(labelTexture_ != nullptr) labelTexture_->Bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
        gc.PopModel();
    }

    void Button::CreateLabel()
    {
        TextRenderer &tr = GameEngine::Get().GetTextRenderer();
        delete labelTexture_;
        tr.SetFGColor(labelColor_.r, labelColor_.g, labelColor_.b, labelColor_.a);
        labelTexture_ = tr.RenderText(font_, text_, false);
        if(labelTexture_ == nullptr)
            return;
        float labelWidth = (float)labelTexture_->GetWidth();
        float labelHeight = (float)labelTexture_->GetHeight();
        float buttonWidth = labelWidth + padding_ * 2.f;
        float buttonHeight = labelHeight + padding_ * 2.f;
        CreateRectangle(buttonWidth, buttonHeight, GetColor());
        width_ = (int)buttonWidth;
        height_= (int)buttonHeight;
        const ogl::Vertex::Color WHITE = {255,255,255,255};
        ogl::Vertex vertices[5] = {
            {{0.f,0.f,0.f},  WHITE,  {0.f,0.f}},
            {{0.f,labelHeight,0.f},  WHITE,  {0.f,1.f}},
            {{labelWidth,labelHeight,0.f},  WHITE,  {1.f,1.f}},
            {{labelWidth,0.f,0.f},  WHITE,  {1.f,0.f}},
            {{0.f,0.f,0.f},  WHITE,  {0.f,0.f}},
        };
        delete labelVbo_;
        labelVbo_ = new ogl::VertexBuffer();
        labelVbo_->SetData(sizeof(ogl::Vertex)*5, vertices, GL_STATIC_DRAW);
        delete labelVao_;
        labelVao_ = new ogl::VertexArray();
        ogl::VertexBufferLayout vbl;    ogl::Vertex::PushLayout(vbl);
        labelVao_->AddBuffer(*labelVbo_, vbl);
        CreateBorder();
    }

}}