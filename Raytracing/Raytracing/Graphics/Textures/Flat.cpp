#include "Flat.h"

namespace Graphics {
	namespace Textures {
		Flat::Flat() {
		}
		Flat::~Flat() {
		}
		glm::dvec3 Flat::GetColor(const glm::dvec2& uvCoords) {
			return glm::dvec3(1.f);
		}
	}
}