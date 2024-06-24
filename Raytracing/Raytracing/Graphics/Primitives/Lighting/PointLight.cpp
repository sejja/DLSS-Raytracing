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
		namespace Lighting {
			// ------------------------------------------------------------------------
			/*! Constructor
			*
			*   Sets the default values for the PointLight
			*/ // ---------------------------------------------------------------------
			PointLight::PointLight() noexcept {
				mColor = glm::vec3(1.0f, 1.0f, 1.0f);
				mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
				mIntensity = 1.0f;
			}

			// ------------------------------------------------------------------------
			/*! Destructor
			*
			*   Destroys the point light
			*/ // ---------------------------------------------------------------------
			PointLight::~PointLight() noexcept {}

			// ------------------------------------------------------------------------
			/*! Compute Lighting
			*
			*   Computes the lighting for the given point
			*/ // ---------------------------------------------------------------------
			bool PointLight::ComputeLighting(const Trace::Ray& ray, const glm::vec3& inpoint, const glm::vec3& innormal,
				const std::vector<std::shared_ptr<Composition::Object>>& objlist,
				const std::shared_ptr<Composition::Object>& obj,
				glm::vec3& color, float& intensity) noexcept {

				const float angle = acos(glm::dot(innormal, glm::normalize(mPosition - inpoint)));

				if (angle > 1.5708) return false;
				color = mColor;
				intensity = mIntensity * (1.f - (angle / 1.5708f));
				return true;
			}
		}
	}
}