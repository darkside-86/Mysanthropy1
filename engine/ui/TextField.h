// TextField.h
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

#include "Frame.h"
#include "Label.h"

namespace engine { namespace ui {

    class TextField : public Frame 
    {
    public:
        TextField(Object* parent, int width, int height, const std::string& fontAlias, ogl::Texture* texture);
        virtual ~TextField();
        virtual void Render(GraphicsContext& gc) override;

        void SetText(const std::string& text) { text_ = text; CreateText(); }
        std::string GetText() { return text_; }

        virtual void OnKeypressed(const KeypressedEvent& e) override;

        void SetEditable(bool e) { editable_ = e; }
        bool IsEditable() { return editable_; }

        void SetWrap(bool w) { wrap_ = w; }
        bool IsWrap() { return wrap_; }

    private:
        void CreateText();
        std::string text_;
        std::vector<Label*> lines_;
        std::string fontAlias_;
        bool editable_ = true;
        bool wrap_ = true;
    };

}}