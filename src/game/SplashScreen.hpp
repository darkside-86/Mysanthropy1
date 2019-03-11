// SplashScreen.hpp
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

#include "engine/ui/Button.hpp"
#include "engine/ui/Frame.hpp"
#include "engine/ui/Label.hpp"
#include "engine/ui/TextField.hpp"
#include "engine/ui/Texture.hpp"
#include "ogl/Program.hpp"
#include "ogl/Texture.hpp"
#include "ogl/VertexArray.hpp"
#include "ogl/VertexBuffer.hpp"

namespace game
{
    // represents the menu selection options made at the splash screen
    class GameLoadState 
    { public:
        // save slot to be read from and written to during course of gameplay
        std::string saveName;
        // whether or not a new game is to be started or if false, a game slot is loaded
        bool newGame;
        // if true, the "boy" character avatar is selected upon new game, else "girl". This
        //  field is meaningless if newGame is false.
        bool boyCharacter;
    };

    // handles starting menu UI and intro screen before game starts
    class SplashScreen
    {
    public:
        // enum indicating the state of the splash screen
        enum SCREEN_STATE { MENU, ENDSPLASH };

        // ctor
        SplashScreen();
        // dtor
        virtual ~SplashScreen();
        // Initializes music
        void Initialize();
        // Updates the splash screen
        void Update(float dtime);
        // Renders splash screen background
        void Render(ogl::Program& program);
        // Stops music
        void Cleanup();
        // Returns the screen state
        inline SCREEN_STATE GetScreenState() const { return screenState_; }
        // Returns the game load state
        inline GameLoadState GetGameLoadState() const { return gameLoadState_; }
    private:
        // replaces any characters invalid in Windows filenames with '_'
        std::string ReplaceInvalidChars(const std::string& str);
        // the ogl objects for rendering the fullscreen background
        ogl::VertexArray bgVao_; 
        // see above
        ogl::VertexBuffer bgVbo_;
        // The texture of the background
        ogl::Texture* bgTexture_;
        // indicates whether or not the splash screen cycle should stop and game begin
        SCREEN_STATE screenState_ = SCREEN_STATE::MENU;
        // What to do when menu is exited and game begins to load
        GameLoadState gameLoadState_;
        // hardcoded UI objects
        engine::ui::Frame* mainScreenPanel_;
        engine::ui::Texture* titleIcon_;
        engine::ui::Texture* newGameIcon_;
        engine::ui::Texture* loadGameIcon_;

        engine::ui::Frame* newScreenPanel_;
        engine::ui::Texture* newGoBackIcon_;
        engine::ui::Frame* newOptionsPanel_;
        engine::ui::Label* newOptionsLabel1_;
        engine::ui::Button* newOptionsBoyBtn_;
        engine::ui::Button* newOptionsGirlBtn_;
        engine::ui::Label* newOptionsLabel2_;
        engine::ui::TextField* newOptionsNameTf_;
        engine::ui::Button* newOptionsStartBtn_;
        engine::ui::Label* newOptionsErrorLbl_;

        engine::ui::Frame* loadScreenPanel_;
        engine::ui::Texture* loadGoBackIcon_;
        engine::ui::Frame* loadOptionsPanel_;
        engine::ui::Button* loadOptionsLoadBtn_;
        engine::ui::Frame* loadOptionsFileListPanel_;
        std::vector<engine::ui::Label*> loadOptionsFileList_;
    };

}