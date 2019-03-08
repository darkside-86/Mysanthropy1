// TextField.hpp
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
#pragma once

#include <vector>

#include "Frame.hpp"
#include "Label.hpp"

namespace engine { namespace ui {

    // Defines a (by default) yellow box in which user can enter text. TODO: cursor and cursor placement
    // support and filtering garbage text input. Already supports backspacing.
    class TextField : public Frame 
    {
    public:
        // Constructor
        TextField(Object* parent, int width, int height, const std::string& fontAlias, ogl::Texture* texture);
        // Destructor
        virtual ~TextField();
        // Renders the box and its containing text
        virtual void Render(GraphicsContext& gc) override;
        // Sets the text to be rendered inside the box
        inline void SetText(const std::string& text) { text_ = text; CreateText(); }
        // Returns the text inside the box as a string
        inline std::string GetText() const { return text_; }
        // Processes the keyboard handler events as usual but also processes keystrokes to add or delete text
        virtual void OnKeypressed(const KeypressedEvent& e) override;
        // Set whether text box can be edited by user input
        inline void SetEditable(bool e) { editable_ = e; }
        // Returns whether or not text can be edited by user input
        inline bool IsEditable() const { return editable_; }
        // Sets whether or not text should be wrapped. TODO: Wrap functionality missing
        inline void SetWrap(bool w) { wrap_ = w; }
        // Returns true if text is to be wrapped. TODO: Wrap functionality missing
        inline bool IsWrap() const { return wrap_; }
    private:
        // (Re)creates opengl objects for when text or textfield box is modified
        void CreateText();
        // The text inside the box
        std::string text_;
        // Lines generated by text containing newline chars. TODO: Generate lines via wrapping functionality
        std::vector<Label*> lines_;
        // Alias of font loaded with TextRenderer
        std::string fontAlias_;
        // Whether or not text can be modified by user input
        bool editable_ = true;
        // Whether or not text should automatically wrap. TODO: Missing wrap functionality
        bool wrap_ = true;
    };

}}