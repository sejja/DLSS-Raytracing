#ifndef _POINT_LIGHT__H_
#define _POINT_LIGHT__H_

#include "Light.h"

namespace Graphics {
	namespace Primitives {
		class PointLight : public Light {
		public:
			PointLight();
			virtual ~PointLight() override;
			bool ComputeLighting(const Trace::Ray& ray, const glm::vec3& inpoint, const glm::vec3& innormal,
				const std::vector<std::shared_ptr<Composition::Object>>& objlist,
				const std::shared_ptr<Composition::Object>& obj,
				glm::vec3& color, float& intensity) override;
		};
	}
}

#endif