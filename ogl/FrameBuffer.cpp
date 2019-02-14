// FrameBuffer.cpp
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

#include "FrameBuffer.h"

#include <GL/glew.h>

namespace ogl
{
    FrameBuffer::FrameBuffer()
    {
        glGenFramebuffers(1, &id_);
        glBindFramebuffer(GL_FRAMEBUFFER, id_);
    }

    FrameBuffer::~FrameBuffer()
    {
        glDeleteFramebuffers(1, &id_);
        glDeleteTextures(1, &tcb_);
    }

    void FrameBuffer::AttachNewTexture(int width, int height)
    {
        Bind();
        glGenTextures(1, &tcb_);
        glBindTexture(GL_TEXTURE_2D, tcb_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tcb_, 0);
    }

    void FrameBuffer::AttachNewRenderBuffer(int width, int height)
    {
        Bind();
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }

    bool FrameBuffer::IsComplete()
    {
        Bind();
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    void FrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, id_);
    }

    void FrameBuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::BindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, tcb_);
    }

    void FrameBuffer::UnbindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}