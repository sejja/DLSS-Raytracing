#pragma once

#include "../../Composition/Object.h"

namespace Graphics {
	namespace Shapes {
		class Cylinder : public Composition::Object {
		public:
			// Default constructor.
			Cylinder();

			// Override the destructor.
			virtual ~Cylinder() override;

			// Override the function to test for intersections.
			virtual bool TestIntersection(const Trace::Ray& castRay, glm::dvec3& intPoint,
				glm::dvec3& localNormal, glm::dvec3& localColor) noexcept override;
		};
	}
}