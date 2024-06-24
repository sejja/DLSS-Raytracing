#ifndef _LIGHT__H_
#define _LIGHT__H_

#include <memory>
#include <vector>
#include "../../Composition/Object.h"
#include "../../Trace/Ray.h"

namespace Graphics {
	namespace Primitives {
		class Light {
		public:
			Light();
			virtual ~Light();

			virtual bool ComputeLighting(const Trace::Ray& ray, const glm::vec3& inpoint, const glm::vec3& innormal, 
													const std::vector<std::shared_ptr<Composition::Object>>& objlist, 
													const std::shared_ptr<Composition::Object>& obj,
													glm::vec3& color, float& intensity);

		public:
			glm::vec3 mColor;
			glm::vec3 mLocation;
			float mIntensity;
		};
	}
}

#endif