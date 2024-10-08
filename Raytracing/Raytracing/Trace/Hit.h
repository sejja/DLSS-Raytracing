#pragma once
#include <glm/glm.hpp>

namespace Trace {
	struct Hit {
		Hit();
		Hit(bool b);
		Hit(bool b, glm::dvec2 uv);

		bool hit;
		glm::dvec2 uUV;
	};
}