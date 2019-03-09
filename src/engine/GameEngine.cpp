// GameEngine.cpp
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
#include "GameEngine.hpp"

#include <chrono>
#include <fstream>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

static SDL_Window* window = nullptr;
static SDL_GLContext context = nullptr;

namespace engine 
{
    bool GameEngine::Initialize(const char* title, int width, int height)
    {
        width_ = width;
        height_ = height;
        if(SDL_Init(SDL_INIT_EVERYTHING) !=0 )
            return false;

        // initialize TTF
        if(TTF_Init() != 0)
            return false;
        
        // initialize mixer (todo: proper error checking and logging)
        Mix_Init(MIX_INIT_FLAC|MIX_INIT_MID|MIX_INIT_MOD|MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_OPUS);
        Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
        Mix_AllocateChannels(4);
        
        // opengl 3.3 core mode
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
        // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 4);

        // create window
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if(window == nullptr)
            return false;

        // create context
        context = SDL_GL_CreateContext(window);
        if(context == nullptr)
            return false;

        // init glew
        GLenum err = glewInit();
		if (err != GLEW_OK)
            return false;

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f); // dark
		glClearDepth(1.0);
		glEnable(GL_DEPTH_TEST); // disable for 2D rendering
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xff);
        glStencilFunc(GL_NOTEQUAL, 1, 0xff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
        // glEnable(GL_MULTISAMPLE);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);  
        // glFrontFace(GL_CCW);    
        glViewport(0, 0, width, height);

        // seed random number gen
        auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
        rng_.seed((unsigned int)seed);

        return true;
    }

    void GameEngine::Cleanup()
    {
        Mix_CloseAudio();
        Mix_Quit();
        // TTF_Quit();
        SDL_Quit();
    }

    void GameEngine::StartGameLoop(Game& game)
    {
        bool quit = false;
        GraphicsContext gc;

		double numFrames = 0;
		double numSeconds = 0;

		long prevTick = 0;
		long currentTick = SDL_GetTicks();
		float dTime = 0.0;

        while(!quit)
        {
            SDL_Event e;
            while(SDL_PollEvent(&e))
            {
                switch(e.type)
                {
                case SDL_QUIT:
                    quit = true;
                    for(auto eachListener : quitListeners_)
                        quit = quit && eachListener(e.quit);
                    break;
                case SDL_WINDOWEVENT:
                    if(e.window.event == SDL_WINDOWEVENT_RESIZED)
                        glViewport(0,0,e.window.data1, e.window.data2);
                    break;
                case SDL_KEYDOWN: case SDL_KEYUP:
                    for(auto eachListener : keyboardListeners_)
                        eachListener(e.key);
                    break;
                case SDL_MOUSEBUTTONDOWN : case SDL_MOUSEBUTTONUP:
                    for(auto el : mouseButtonListeners_)
                        el(e.button);
                    break;
                case SDL_MOUSEMOTION:
                    for(auto el : mouseMotionListeners_)
                        el(e.motion);
                    break;
                }
            }
            prevTick = currentTick;
			currentTick = SDL_GetTicks();
			dTime = (float)(currentTick - prevTick) / 1000.f;
            // calc fps
            numFrames += 1;
            numSeconds += (double)dTime;
            framesPerSecond_ = numFrames / numSeconds;
            if(dTime > 0.2f) // clamp maximum time elapsed to N seconds
                dTime = 0.2f;
            game.Update(dTime);
            glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
            game.Render(gc);
            SDL_GL_SwapWindow(window);
        }
    }

    void GameEngine::GetWindowSize(int& x, int &y)
    {
        SDL_GetWindowSize(window, &x, &y);
    }

    void GameEngine::SetLogicalXY(int& x, int& y)
    {
        if(window == nullptr)
        {
            x = 0;
            y = 0;
            return;
        }
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        float scaleX = (float)width_ / (float)windowWidth;
        float scaleY = (float)height_ / (float)windowHeight;
        x = (int)((float)x * scaleX);
        y = (int)((float)y * scaleY);
    }

    void GameEngine::SetActualScale(float& x, float& y)
    {
        if(window == nullptr)
        {
            x = 0.f;
            y = 0.f;
            return;
        }
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        float scaleX = (float)windowWidth /  (float)width_;
        float scaleY = (float)windowHeight / (float)height_;
    }

    std::string GameEngine::ReadFileAsString(const std::string& path) const
    {
        std::ifstream inFile;
        inFile.open(path);
        if(!inFile.is_open())
        {
            logger_.Logf(Logger::Severity::WARNING, "%s: Unable to read file `%s'", 
                    __FUNCTION__, path.c_str());
            return "";
        }
        std::string str, result;
        while( std::getline(inFile, str) )
            result += str + "\n";
        inFile.close();
        return result;
    }

    void GameEngine::Rotate2DVector(float &x, float &y, const float degrees) const
    {
        float x0 = x;
        float y0 = y;
        float theta = glm::radians(degrees);

        float x1 = glm::cos(theta) * x0 - glm::sin(theta) * y0;
        float y1 = glm::sin(theta) * x0 + glm::cos(theta) * y0;
        x = x1;
        y = y1;
    }

    std::string GameEngine::FormatTimeInSeconds(int seconds) const
    {
        std::string formatted = "";
        const int MINUTES = 60;
        const int HOURS = 60 * MINUTES;
        int hours = seconds / HOURS;
        if(hours > 0)
        {
            formatted += std::to_string(hours) + " hours, ";
            seconds -= hours * HOURS; // leave only minutes and seconds remaining for calculation
        }
        int minutes = seconds / MINUTES;
        if( minutes > 0 )
        {
            formatted += std::to_string(minutes) + " minutes, and ";
            seconds -= minutes * MINUTES;
        }
        formatted += std::to_string(seconds) + " seconds";
        return formatted;
    }

    bool GameEngine::PercentChance(float percent)
    {
        float result = 100.0f * (float)rng_() / (float)rng_.max();
        return result < percent;
    }

}