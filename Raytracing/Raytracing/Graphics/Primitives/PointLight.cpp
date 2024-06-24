//
//	PointLight.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 24/06/24
//	Copyright © 2024. All Rights reserved
//

#include "PointLight.h"

namespace Graphics {
	namespace Primitives {
		PointLight::PointLight() {
			mColor = glm::vec3(1.0f, 1.0f, 1.0f);
			mLocation = glm::vec3(0.0f, 0.0f, 0.0f);
			mIntensity = 1.0f;
		}

		PointLight::~PointLight() {
		}

		bool PointLight::ComputeLighting(const Trace::Ray& ray, const glm::vec3& inpoint, const glm::vec3& innormal,
			const std::vector<std::shared_ptr<Composition::Object>>& objlist,
			const std::shared_ptr<Composition::Object>& obj,
			glm::vec3& color, float& intensity) {
			glm::vec3 lightdir = glm::normalize(mLocation - inpoint);
			float angle = acos(glm::dot(innormal, lightdir));

			if (angle > 1.5708) {
				color = mColor;
				intensity = 0.0f;
				return false;
			}
			else {
				color = mColor;
				intensity = mIntensity * (1.0 - (angle / 1.5708));
				return true;
			}
		}
	}
}