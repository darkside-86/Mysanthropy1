// SplashScreen.cpp
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

#include <filesystem>

#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.hpp"
#include "engine/ui/Root.hpp"
#include "ogl/Vertex.hpp"
#include "SplashScreen.hpp"

SplashScreen::SplashScreen()
{
    // define the vertices for the background.
    float scrW = (float)engine::GameEngine::Get().GetWidth();
    float scrH = (float)engine::GameEngine::Get().GetHeight();

    ogl::Vertex vertices[6] = {
        {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.f,0.f}, {0.f,0.f,1.f}}, // top left
        {{0.f,scrH, 0.f}, {255,255,255,255}, {0.f,1.f}, {0.f,0.f,1.f}}, // bottom left
        {{scrW,scrH, 0.f}, {255,255,255,255}, {1.f,1.f}, {0.f,0.f,1.f}}, // bottom right
        {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.f,0.f}, {0.f,0.f,1.f}}, // top left
        {{scrW,scrH, 0.f}, {255,255,255,255}, {1.f,1.f}, {0.f,0.f,1.f}}, // bottom right
        {{scrW, 0.f, 0.f}, {255,255,255,255}, {1.f,0.f}, {0.f,0.f,1.f}}, // top right
    };

    bgVbo_.SetData(sizeof(ogl::Vertex)*6, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    bgVao_.AddBuffer(bgVbo_, vbl);

    // load the background texture of the splashscreen
    bgTexture_ = new ogl::Texture("res/textures/splash.png", false, false);

    // initialize main UI
    int iScreenWidth = (int)scrW;
    int iScreenHeight = (int)scrH;
    
    ogl::Texture* uiblank = engine::GameEngine::Get().GetTextureManager().GetTexture("res/textures/uiblank.png");
    auto& tr = engine::GameEngine::Get().GetTextRenderer();
    tr.LoadFont("res/fonts/Vollkorn-Regular.ttf", "vollkorn25", 50);
    // setup the main starting panel //////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    mainScreenPanel_ = new engine::ui::Frame(engine::ui::Root::Get(), iScreenWidth, iScreenHeight, 0, 0,
        uiblank, {0.0f, 0.0f, 0.0f, 0.0f});
    titleIcon_ = new engine::ui::Texture(mainScreenPanel_, "res/textures/textTitle.png");
    titleIcon_->SetXPos(iScreenWidth / 2 - titleIcon_->GetWidth() / 2);
    titleIcon_->SetYPos(10);
    newGameIcon_ = new engine::ui::Texture(mainScreenPanel_, "res/textures/textNewGame.png");
    newGameIcon_->SetXPos(iScreenWidth / 2 - newGameIcon_->GetWidth() / 2);
    newGameIcon_->SetYPos(2 * iScreenHeight / 3 - newGameIcon_->GetHeight() / 2);
    newGameIcon_->AddOnClicked([this](const engine::ui::ClickedEvent& e){
        mainScreenPanel_->SetVisible(false);
        newScreenPanel_->SetVisible(true);
    });
    loadGameIcon_ = new engine::ui::Texture(mainScreenPanel_, "res/textures/textLoadGame.png");
    loadGameIcon_->SetXPos(iScreenWidth / 2 - loadGameIcon_->GetWidth() / 2);
    loadGameIcon_->SetYPos(newGameIcon_->GetYPos() + newGameIcon_->GetHeight() + 20);
    loadGameIcon_->AddOnClicked([this](const engine::ui::ClickedEvent& e){
        mainScreenPanel_->SetVisible(false);
        loadScreenPanel_->SetVisible(true);
    });
    // setup the "New game" screen ////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    newScreenPanel_ = new engine::ui::Frame(engine::ui::Root::Get(), iScreenWidth, iScreenHeight, 0, 0,
        uiblank, {0.0f, 0.0f, 0.0f, 0.0f});
    newScreenPanel_->SetVisible(false);
    newGoBackIcon_ = new engine::ui::Texture(newScreenPanel_, "res/textures/textGoBack.png");
    newGoBackIcon_->SetXPos(10); newGoBackIcon_->SetYPos(10);
    newGoBackIcon_->AddOnClicked([this](const engine::ui::ClickedEvent& e){
        newScreenPanel_->SetVisible(false);
        mainScreenPanel_->SetVisible(true);
    });
    newOptionsPanel_ = new engine::ui::Frame(newScreenPanel_, 3 * iScreenWidth / 4, 3 * iScreenHeight / 4, 0, 0,
        uiblank, {0.1f, 0.1f, 0.1f, 0.4f});
    newOptionsPanel_->SetXPos(iScreenWidth / 2 - newOptionsPanel_->GetWidth() / 2);
    newOptionsPanel_->SetYPos(iScreenHeight / 2 - newOptionsPanel_->GetHeight() / 2);
    newOptionsLabel1_ = new engine::ui::Label(newOptionsPanel_, "I am a...", "vollkorn25", {1.f,1.f,1.f,1.f});
    newOptionsBoyBtn_ = new engine::ui::Button(newOptionsPanel_, uiblank, {0.0f, 0.0f, 1.0f, 1.0f}, "Boy", 
        "vollkorn25", {1.0f,1.0f,1.0f,1.0f}, 1);
    newOptionsBoyBtn_->SetBorderColor({1.0f,1.0f,1.0f,1.0f});
    newOptionsBoyBtn_->SetBorderSize(1);
    newOptionsBoyBtn_->SetXPos(newOptionsLabel1_->GetXPos() + newOptionsLabel1_->GetWidth() + 20);
    newOptionsBoyBtn_->SetYPos(newOptionsLabel1_->GetYPos());
    gameLoadState_.boyCharacter = true;
    newOptionsBoyBtn_->AddOnClicked([this](const engine::ui::ClickedEvent& e){
        gameLoadState_.boyCharacter = true;
        newOptionsBoyBtn_->SetColor({0.f, 0.f, 1.f, 1.f});
        newOptionsGirlBtn_->SetColor({0.0f, 0.0f, 0.0f, 0.0f});
    });
    newOptionsGirlBtn_ = new engine::ui::Button(newOptionsPanel_, uiblank, {0.0f, 0.0f, 0.0f, 0.0f}, "Girl",
        "vollkorn25", {1.0f, 1.0f, 1.0f, 1.0f}, 1);
    newOptionsGirlBtn_->SetBorderColor({1.0f, 1.0f, 1.0f, 1.0f});
    newOptionsGirlBtn_->SetBorderSize(1);
    newOptionsGirlBtn_->SetXPos(newOptionsBoyBtn_->GetXPos() + newOptionsBoyBtn_->GetWidth() + 20);
    newOptionsGirlBtn_->SetYPos(newOptionsBoyBtn_->GetYPos());
    newOptionsGirlBtn_->AddOnClicked([this](const engine::ui::ClickedEvent& e){
        gameLoadState_.boyCharacter = false;
        newOptionsGirlBtn_->SetColor({1.0f, 0.4f, 0.4f, 1.0f});
        newOptionsBoyBtn_->SetColor({0.0f, 0.0f, 0.0f, 0.0f});
    });
    newOptionsLabel2_ = new engine::ui::Label(newOptionsPanel_, "My name is...", "vollkorn25", 
        {1.f, 1.f, 1.f, 1.f});
    newOptionsLabel2_->SetXPos(newOptionsLabel1_->GetXPos());
    newOptionsLabel2_->SetYPos(newOptionsGirlBtn_->GetYPos() + newOptionsGirlBtn_->GetHeight() + 20);
    newOptionsNameTf_ = new engine::ui::TextField(newOptionsPanel_, 
        newOptionsPanel_->GetWidth() - newOptionsLabel2_->GetWidth() - 21, 60, "vollkorn25", uiblank);
    newOptionsNameTf_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
    newOptionsNameTf_->SetXPos(newOptionsLabel2_->GetXPos() + newOptionsLabel2_->GetWidth() + 20);
    newOptionsNameTf_->SetYPos(newOptionsLabel2_->GetYPos());
    newOptionsStartBtn_ = new engine::ui::Button(newOptionsPanel_, uiblank, {0.0f, 0.0f, 0.0f, 0.0f}, "Start",
        "vollkorn25", {1.0f, 1.0f, 1.0f, 1.0f}, 20);
    newOptionsStartBtn_->SetBorderSize(1);
    newOptionsStartBtn_->SetBorderColor({1.0f,1.0f,1.0f,1.0f});
    newOptionsStartBtn_->SetXPos(newOptionsPanel_->GetWidth() / 2 - newOptionsStartBtn_->GetWidth() / 2);
    newOptionsStartBtn_->SetYPos(newOptionsNameTf_->GetYPos() + newOptionsNameTf_->GetHeight() + 20);
    newOptionsStartBtn_->AddOnClicked([this](const engine::ui::ClickedEvent& e){
        gameLoadState_.newGame = true;
        gameLoadState_.saveName = newOptionsNameTf_->GetText();
        // validate name to make sure it's not "in use"
        std::string desiredName = std::string("TileGame/saves/") + gameLoadState_.saveName + ".sav";
        std::experimental::filesystem::path path(desiredName);
        if(std::experimental::filesystem::exists(path) || desiredName == "")
        {
            newOptionsErrorLbl_->SetVisible(true);
        }
        else 
        {
            // make sure name is stripped of invalid filename chars and proceed to game
            gameLoadState_.saveName = ReplaceInvalidChars(gameLoadState_.saveName);
            screenState_ = SCREEN_STATE::ENDSPLASH;
        }
    });
    newOptionsErrorLbl_ = new engine::ui::Label(newOptionsPanel_, "Name is already in use!", 
        "vollkorn25", {1.0f, 0.0f, 0.0f, 1.0f});
    newOptionsErrorLbl_->SetXPos(newOptionsPanel_->GetWidth() / 2 - newOptionsErrorLbl_->GetWidth() / 2);
    newOptionsErrorLbl_->SetYPos(newOptionsStartBtn_->GetYPos() + newOptionsStartBtn_->GetHeight() + 20);
    newOptionsErrorLbl_->SetVisible(false);
    // setup the "Load game" screen ///////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    loadScreenPanel_ = new engine::ui::Frame(engine::ui::Root::Get(), iScreenWidth, iScreenHeight, 0, 0,
        uiblank, {0.0f, 0.0f, 0.0f, 0.0f});
    loadScreenPanel_->SetVisible(false);
    loadGoBackIcon_ = new engine::ui::Texture(loadScreenPanel_, "res/textures/textGoBack.png");
    loadGoBackIcon_->SetXPos(10); loadGoBackIcon_->SetYPos(10);
    loadGoBackIcon_->AddOnClicked([this](const engine::ui::ClickedEvent& e){
        loadScreenPanel_->SetVisible(false);
        mainScreenPanel_->SetVisible(true);
    });
    loadOptionsPanel_ = new engine::ui::Frame(loadScreenPanel_, 3 * iScreenWidth / 4, 
        3 * iScreenHeight / 4, 0, 0, uiblank, {0.1f, 0.1f, 0.1f, 0.4f});
    loadOptionsPanel_->SetXPos(iScreenWidth / 2 - loadOptionsPanel_->GetWidth() / 2);
    loadOptionsPanel_->SetYPos(iScreenHeight / 2 - loadOptionsPanel_->GetHeight() / 2);
    loadOptionsLoadBtn_ = new engine::ui::Button(loadOptionsPanel_, uiblank, {0.0f, 0.0f, 0.0f, 0.0f},
        "Load Game", "vollkorn25", {1.0f, 1.0f, 1.0f, 1.0f}, 10);
    loadOptionsLoadBtn_->SetBorderColor({1.0f, 1.0f, 1.0f, 1.0f});
    loadOptionsLoadBtn_->SetBorderSize(1);
    loadOptionsLoadBtn_->AddOnClicked([this](const engine::ui::ClickedEvent& e){
        gameLoadState_.newGame = false;
        if(gameLoadState_.saveName != "")
        {
            // valid save slot selection so begin game
            screenState_ = SCREEN_STATE::ENDSPLASH;
        }
        // silently ignore lack of selection
    });
    loadOptionsFileListPanel_ = new engine::ui::Frame(loadOptionsPanel_, loadOptionsPanel_->GetWidth(),
        loadOptionsPanel_->GetHeight() - loadOptionsLoadBtn_->GetHeight(), 0, 
        loadOptionsLoadBtn_->GetHeight(), uiblank, {0.0f, 0.0f, 0.0f, 0.0f});
    loadOptionsFileListPanel_->AddOnDragged([this](const engine::ui::DraggedEvent& e){
        // implement drag scrolling
        for(auto each : loadOptionsFileList_)
        {
            each->SetYPos(each->GetYPos() + e.yrel);
        }
    });
    // generate vector of labels corresponding to slot name.
    std::experimental::filesystem::path saveDir("TileGame/saves");
    int y = 0;
    for(const auto& each : std::experimental::filesystem::directory_iterator(saveDir))
    {
        std::string fileName = each.path().filename().string();
        auto dot = fileName.find_last_of('.');
        fileName = fileName.substr(0, dot);
        engine::ui::Label* label = new engine::ui::Label(loadOptionsFileListPanel_, fileName, "vollkorn25",
            {1.0f, 1.0f, 0.3f, 1.0f});
        loadOptionsFileList_.push_back(label);
        label->SetYPos(y);
        y += label->GetHeight();
    }
    // file save label selector
    loadOptionsFileListPanel_->AddOnClicked([this](const engine::ui::ClickedEvent& e){
        // determine what label was clicked if any
        engine::ui::Label* clicked = nullptr;
        for(auto each : loadOptionsFileList_)
        {
            each->SetColor({1.0f, 1.0f, 0.3f, 1.0f}); // reset colors
            if(e.x >= each->GetXPos() && e.x <= each->GetXPos() + each->GetWidth() &&
              e.y >= each->GetYPos() && e.y <= each->GetYPos() + each->GetHeight())
            {
                clicked = each; // only one will be "clicked" because elements never overlap
                // and we can't "break;" because we have to reset the colors of each element
            }
        }
        if(clicked != nullptr)
        {
            gameLoadState_.saveName = clicked->GetText();
            clicked->SetColor({0.1f, 0.8f, 0.1f, 1.f});
        }
        else
        {
            gameLoadState_.saveName = "";
        }
    });

}

SplashScreen::~SplashScreen()
{
    delete bgTexture_;
    delete mainScreenPanel_;
     delete titleIcon_;
     delete newGameIcon_;
     delete loadGameIcon_;
    delete newScreenPanel_;
     delete newGoBackIcon_;
     delete newOptionsPanel_;
      delete newOptionsLabel1_;
      delete newOptionsBoyBtn_;
      delete newOptionsGirlBtn_;
      delete newOptionsLabel2_;
      delete newOptionsNameTf_;
      delete newOptionsStartBtn_;
      delete newOptionsErrorLbl_;
    delete loadScreenPanel_;
     delete loadGoBackIcon_;
     delete loadOptionsPanel_;
      delete loadOptionsLoadBtn_;
      delete loadOptionsFileListPanel_;
      for(auto each : loadOptionsFileList_)
      {
          delete each;
      }
    auto& tr = engine::GameEngine::Get().GetTextRenderer();
    tr.UnloadFont("vollkorn25");
}

void SplashScreen::Initialize()
{
    engine::GameEngine::Get().GetSoundManager().PlayMusic("res/music/island3.ogg", -1);
}

void SplashScreen::Update(float dtime)
{

}

void SplashScreen::Render(ogl::Program& program)
{
    glm::mat4 model(1.f), view(1.f), projection(1.f);
    float scrW = (float)engine::GameEngine::Get().GetWidth();
    float scrH = (float)engine::GameEngine::Get().GetHeight();

    projection = glm::ortho(0.f, scrW, scrH, 0.f);
    // first draw the background fullscreen image
    program.Use();
    program.SetUniform<int>("u_useTexture", true);
    program.SetUniform<int>("u_useColorBlending", false);
    program.SetUniform("u_model", model);
    program.SetUniform("u_view", view);
    program.SetUniform("u_projection", projection);
    bgVao_.Bind();
    bgTexture_->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SplashScreen::Cleanup()
{
    engine::GameEngine::Get().GetSoundManager().StopMusic();
}

std::string SplashScreen::ReplaceInvalidChars(const std::string& str)
{
    std::string copy = str;
    for(int i=0; i < str.length(); ++i)
    {
        // It cannot be any of \ / : * ? " < > |
        switch(copy[i])
        {
            case '\\': case '/': case ':': case '*': case '?': case '"':
            case '<': case '>': case '|':
                copy[i] = '_';
        }
    }
    return copy;
}