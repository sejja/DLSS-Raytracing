#include "Checker.h"

namespace Graphics {
	namespace Textures {
		Checker::Checker() {
		}
		Checker::~Checker() {
		}
		glm::dvec3 Checker::GetColor(const glm::dvec2& uvCoords) {
			glm::dvec2 inputLoc = uvCoords;
			glm::dvec3 newLoc = ApplyTransform(glm::dvec3(inputLoc, 0.f));
			double newU = newLoc.x;
			double newV = newLoc.y;

			glm::dvec3 localColor{ 4 };
			int check = static_cast<int>(floor(newU)) + static_cast<int>(floor(newV));

			if ((check % 2) == 0)
			{
				localColor = glm::dvec3(1.f);
			}
			else
			{
				localColor = glm::dvec3(0.f);
			}

			return localColor;
		}
	}
}