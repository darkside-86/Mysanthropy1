// Button.h
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

#include <string>

#include "Color.h"
#include "Frame.h"
#include "ogl/Texture.h"
#include "ogl/VertexArray.h"
#include "ogl/VertexBuffer.h"

namespace engine { namespace ui {

    // Defines a rectangle colored shape containing colored text.
    class Button : public Frame
    {
    public:
        // Constructor. fontAlias should be the alias for an already loaded font. See engine::TextRenderer.
        // padding is the space between the edges of the button and the inner text.
        Button(Object* parent, ogl::Texture* texture, const Color& color, 
               const std::string& text, const std::string fontAlias, 
               const Color& textColor, int padding);
        // Destructor
        virtual ~Button();
        // Render the button in 2D space using the GraphicsContext's shader
        virtual void Render(GraphicsContext& gc) override;
        // Sets the inner text of the button
        inline void SetText(const std::string& text) { text_ =text; CreateLabel(); }
        // Sets the fill color of the rectangle
        inline void SetTextColor(const Color& color) { labelColor_ = color; CreateLabel(); }
    private:
        // Recreate opengl objects for the inner label
        void CreateLabel();
        // The inner text of the button
        std::string text_;
        // The font alias used to render the text in
        std::string font_;
        // The color of the text
        Color labelColor_;
        // The spacing between edge and text
        int padding_;
        // The opengl objects for the inner label
        ogl::VertexArray    *labelVao_ = nullptr;
        ogl::VertexBuffer   *labelVbo_ = nullptr;
        // The texture of the inner label. Generated with the TextRenderer
        ogl::Texture        *labelTexture_ = nullptr;
    };

}}