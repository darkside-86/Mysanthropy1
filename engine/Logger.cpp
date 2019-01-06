// Logger.cpp
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
#include "Logger.h"

#include <cassert>
#include <cstdarg>
#include <cstdio>

static FILE* fp = nullptr;

namespace engine
{
    Logger::Logger()
    {
        if(fp == nullptr)
        {
            auto errNo = fopen_s(&fp, "log.txt", "w");
            if(errNo != 0)
            {
                fprintf(stderr, "%s: Warning, cannot open log file\n", __FUNCTION__);
                fflush(stderr);
                fp = nullptr;
            }
        }
        else
        {
            fprintf(stderr, "%s: Warning, log file already open\n", __FUNCTION__);
            fflush(stderr);
        }
    }

    Logger::~Logger()
    {
        fclose(fp);
        fp = nullptr;
    }

    // todo: mirror output to file
    void Logger::Logf(const Severity& sev, const char* format, ...)
    {
        const unsigned int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        va_list argList;
        va_start(argList, format);
        vsnprintf(buffer, BUFFER_SIZE, format, argList);
        switch(sev)
        {
        case Severity::FATAL:
            fprintf(stderr, "[FATAL] %s\n", buffer); 
            if(fp) fprintf(fp, "[FATAL] %s\n", buffer);
            fflush(stderr);
            assert(false);
            break;
        case Severity::INFO:
            fprintf(stdout, "[INFO] %s\n", buffer);
            if(fp) fprintf(fp, "[INFO] %s\n", buffer);
            fflush(stdout);
            break;
        case Severity::WARNING:
            fprintf(stderr, "[WARNING] %s\n", buffer); 
            if(fp) fprintf(fp, "[WARNING] %s\n", buffer);
            fflush(stderr);
            break;
        }
        va_end(argList);
    }
}