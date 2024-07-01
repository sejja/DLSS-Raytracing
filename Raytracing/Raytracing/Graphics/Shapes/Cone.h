#pragma once

#include "../../Composition/Object.h"

namespace Graphics {
	namespace Shapes {
		class Cone : public Composition::Object {
		public:
			// Default constructor.
			Cone();

			// Override the destructor.
			virtual ~Cone() override;

			// Override the function to test for intersections.
			virtual bool TestIntersection(const Trace::Ray& castRay, glm::dvec3& intPoint,
				glm::dvec3& localNormal, glm::dvec3& localColor) noexcept override;
		};
	}
}