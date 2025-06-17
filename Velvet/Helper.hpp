#pragma once

//#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <algorithm>

namespace Velvet
{
	namespace Helper
	{
		glm::mat4 RotateWithDegree(glm::mat4 result, const glm::vec3& rotation);

		glm::vec3 RotateWithDegree(glm::vec3 result, const glm::vec3& rotation);

		float Random(float min = 0, float max = 1);

		glm::vec3 RandomUnitVector();

		template <class T>
		T Lerp(T value1, T value2, float a)
		{
			a = std::min(std::max(a, 0.0f), 1.0f);
			return a * value2 + (1 - a) * value1;
		}
	}
}