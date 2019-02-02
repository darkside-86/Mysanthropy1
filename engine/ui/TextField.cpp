// TextField.cpp
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

#include <SDL/SDL_ttf.h>

#include "../GameEngine.h"
#include "../TextRenderer.h"
#include "TextField.h"

namespace engine { namespace ui {

    static std::vector<std::string> SplitString(std::string str, const std::string& delim)
    {
        std::vector<std::string> strings;
        std::string token;
        size_t pos = 0;
        while( (pos = str.find(delim)) != std::string::npos )
        {
            token = str.substr(0, pos);
            strings.push_back(token);
            str.erase(0, pos + delim.length());
        }
        if(str.length() > 0) // whatever is left
            strings.push_back(str);
        return strings;
    }

    static char ProcessKeystroke(const KeypressedEvent& e)
    {
        if(e.keyCode == SDLK_RETURN)
        {
            return '\n';
        }
        else if(e.mod & KMOD_LSHIFT || e.mod & KMOD_RSHIFT)
        {
            if(e.keyCode >= 11 && e.keyCode <= 255)
            {
                switch(e.keyCode)
                {
                    case 'a': return 'A'; case 'b': return 'B'; case 'c': return 'C'; case 'd': return 'D';
                    case 'e': return 'E'; case 'f': return 'F'; case 'g': return 'G'; case 'h': return 'H';
                    case 'i': return 'I'; case 'j': return 'J'; case 'k': return 'K'; case 'l': return 'L';
                    case 'm': return 'M'; case 'n': return 'N'; case 'o': return 'O'; case 'p': return 'P';
                    case 'q': return 'Q'; case 'r': return 'R'; case 's': return 'S'; case 't': return 'T';
                    case 'u': return 'U'; case 'v': return 'V'; case 'w': return 'W'; case 'x': return 'X';
                    case 'y': return 'Y'; case 'z': return 'Z';
                    case '`': return '~'; case '1': return '!'; case '2': return '@'; case '3': return '#';
                    case '4': return '$'; case '5': return '%'; case '6': return '^'; case '7': return '&';
                    case '8': return '*'; case '9': return '('; case '0': return ')'; case '-': return '_';
                    case '=': return '+'; case '[': return '{'; case ']': return '}'; case '\\': return '|';
                    case ';': return ':'; case '\'': return '\"';
                    case ',': return '<'; case '.': return '>'; case '/': return '?';
                }
            }
            return 0;
        }
        else
        {
            return e.keyCode;
        }
    }

    TextField::TextField(Object* parent, int width, int height, const std::string& fontAlias, ogl::Texture* texture)
            : Frame(parent, width, height, 0, 0, texture, {1.f,1.f,0.5f,1.f}), fontAlias_(fontAlias)
    {
        SetBorderColor({0.f,0.f,0.f,1.f});
        SetBorderSize(1);
    }

    TextField::~TextField()
    {
        for(auto each : lines_)
            delete each;
    }

    void TextField::OnKeypressed(const KeypressedEvent& e)
    {
        if(editable_)
        {
            char c = ProcessKeystroke(e);
            if(e.keyCode == SDLK_BACKSPACE)
            {
                if(text_.length() > 0)
                {
                    text_.erase(text_.length()-1);
                }
            }
            else if(c != 0)
            {
                text_ += c;
            }
            CreateText();
        }
        Object::OnKeypressed(e);
    }
    
    void TextField::Render(GraphicsContext& gc)
    {
        Frame::Render(gc);
    }

    void TextField::CreateText()
    {
        TextRenderer& tr = GameEngine::Get().GetTextRenderer();
        TTF_Font* font = tr.GetFont(fontAlias_);
        if(font == nullptr)
            return;

        // delete existing lines
        for(auto each : lines_)
            delete each;
        lines_.clear();

        // split text by line
        std::vector<std::string> textLines = SplitString(text_, "\n");

        // TODO: if wrap, split further by using TTF_SizeUTF8
        
        int lineY = 0;
        int inc = TTF_FontHeight(font);
        for(auto eachLine : textLines)
        {
            Label *label = new Label(this, eachLine, fontAlias_, {0.f,0.f,0.f,1});
            label->SetYPos(lineY);
            lines_.push_back(label);
            lineY += inc;
        }
    }

}}