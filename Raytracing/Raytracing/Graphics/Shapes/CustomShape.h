#pragma once

#include "../../Composition/Object.h"
#include <vector>
#include <string>

namespace Graphics {
	namespace Shapes {
		class CustomShape : public Composition::Object {
		public:
			// Default constructor.
			CustomShape(const std::string& objFilepath);

			// Override the destructor.
			virtual ~CustomShape() override;

			// Override the function to test for intersections.
			virtual Trace::Hit TestIntersection(const Trace::Ray& castRay, glm::dvec3& intPoint,
				glm::dvec3& localNormal, glm::dvec3& localColor) noexcept override;

		private:
			bool LoadMesh(const std::string& objFilePath);
			std::vector<glm::ivec3> mIndices;
			std::vector<glm::dvec3> mVertices;
		};
	}
}