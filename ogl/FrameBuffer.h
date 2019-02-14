// FrameBuffer.h
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

namespace ogl
{
    class FrameBuffer
    {
    public:
        FrameBuffer();
        ~FrameBuffer();
        void AttachNewTexture(int width, int height);
        void AttachNewRenderBuffer(int width, int height);
        bool IsComplete();
        void Bind();
        void Unbind();
        void BindTexture();
        void UnbindTexture();
    private:
        unsigned int id_;
        unsigned int tcb_;
    };
}