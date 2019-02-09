// Camera.cpp 
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

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine
{

	Camera::Camera() : position_({0.f,0.f,0.f}), direction_({0.f,0.f,-1.f}), up_({0.f,1.0f,0.f})
	{
	}

	Camera::~Camera()
	{
	}

	glm::mat4 Camera::CalculateView()
	{
		glm::mat4 view = glm::lookAt(position_, position_+direction_, up_);
		return view;
	}
}