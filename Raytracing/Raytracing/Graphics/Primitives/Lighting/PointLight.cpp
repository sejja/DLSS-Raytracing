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
			bool PointLight::ComputeLighting(const glm::vec3& inpoint, const glm::vec3& innormal,
				const std::vector<std::shared_ptr<Composition::Object>>& objlist,
				const std::shared_ptr<Composition::Object>& obj,
				glm::vec3& color, float& intensity) noexcept {

				const glm::vec3 lightDir = glm::normalize(mPosition - inpoint);
				const glm::vec3 startPoint = inpoint;
				const Trace::Ray lightRay(startPoint, startPoint + lightDir);

				glm::vec3 poi;
				glm::vec3 poiNormal;
				glm::vec4 poiColor;
				bool validInt = false;

				// Check for intersections with other objects.
				for (auto& sceneObject : objlist) {
					// If the object is not the current object.
					if (sceneObject != obj)
						validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);

					if (validInt) break;
				}

				// If there is no intersection, then we have illumination.
				if (!validInt) {
					const float angle = acos(glm::dot(innormal, lightDir));

					if (angle > 1.5708) {
						// No illumination.
						color = mColor;
						intensity = 0.0;
						return false;
					} else {
						// We do have illumination.
						color = mColor;
						intensity = mIntensity * (1.f - (angle / 1.5708f));
						return true;
					}
				} else {
					// Shadow, so no illumination.
					color = mColor;
					intensity = 0.0;
					return false;
				}
			}
		}
	}
}