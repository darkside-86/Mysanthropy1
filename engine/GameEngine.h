// GameEngine.h
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

#include <random>
#include <vector>

#include "EventListeners.h"
#include "Game.h"
#include "Logger.h"
#include "TextRenderer.h"
#include "TextureManager.h"

namespace engine 
{
    class GameEngine
    {
    public:
        // Initializes SDL and OpenGL
        bool Initialize(const char* title, int width, int height);
        // Cleans up SDL
        void Cleanup();
        // Starts the update and render loop
        void StartGameLoop(Game& game);
        // Get the Singleton instance of GameEngine
        static GameEngine& Get()
        {
            static GameEngine singleton;
            return singleton;
        }
        // Adds an event listener that is called when a keyboard button is pressed or released
        inline void AddKeyboardListener(KeyboardListener kl) 
        {
            keyboardListeners_.push_back(kl);
        }
        // Adds an event listener that is called when a button on the mouse is pressed or released
        inline void AddMouseButtonListener(MouseButtonListener mbl)
        {
            mouseButtonListeners_.push_back(mbl);
        }
        // Adds an event listener that is called when the mouse is moved
        inline void AddMouseMotionListener(MouseMotionListener mml)
        {
            mouseMotionListeners_.push_back(mml);
        }
        // Returns the logical width of the game screen
        inline int GetWidth() { return width_; }
        // Returns the logical height of the game screen
        inline int GetHeight() { return height_; }
        // Returns the logical aspect ratio of the game screen
        inline float GetAspectRatio() { return (float)width_ / (float)height_; }
        // Get the real window pixel dimensions
        void GetWindowSize(int& x, int &y);
        // Translate parameters x and y into logical coordinates. The return
        //  values are stored in the reference parameters.
        void SetLogicalXY(int& x, int& y);
        // Set the actual to logical pixel scale
        void SetActualScale(float& x, float& y);
        // Returns the Logger instance
        inline Logger& GetLogger() { return logger_; }
        // Return the text renderer
        inline TextRenderer& GetTextRenderer() { return textRenderer_; }
        // Get texture manager
        inline TextureManager& GetTextureManager() { return textureManager_; }
        // Return the random number generator
        inline std::default_random_engine& GetRNG() { return rng_; }
    private:
        GameEngine() {}
        ~GameEngine() {}

        std::vector<KeyboardListener> keyboardListeners_;
        std::vector<MouseButtonListener> mouseButtonListeners_;
        std::vector<MouseMotionListener> mouseMotionListeners_;
        int width_, height_; // logical pixels. scalable with window resize
        Logger logger_;
        TextRenderer textRenderer_;
        TextureManager textureManager_;
        std::default_random_engine rng_;
    };
}