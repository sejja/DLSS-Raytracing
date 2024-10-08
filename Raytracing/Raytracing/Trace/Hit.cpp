#include "Hit.h"

namespace Trace {
	Hit::Hit() : hit(false), uUV(0.0, 0.0) {}
	Hit::Hit(bool b) : hit(b), uUV(0.0, 0.0) {}
	Hit::Hit(bool b, glm::dvec2 uv) : hit(b), uUV(uv) {}
}