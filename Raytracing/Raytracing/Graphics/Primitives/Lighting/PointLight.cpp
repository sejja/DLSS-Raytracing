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
				mColor = glm::dvec3(1.0f, 1.0f, 1.0f);
				mPosition = glm::dvec3(0.0f, 0.0f, 0.0f);
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
			bool PointLight::ComputeLighting(const glm::dvec3& inpoint, const glm::dvec3& innormal,
				const std::vector<std::shared_ptr<Composition::Object>>& objlist,
				const std::shared_ptr<Composition::Object>& obj,
				glm::dvec3& color, double& intensity) noexcept {

				const glm::dvec3 lightDir = glm::normalize(mPosition - inpoint);
				const glm::dvec3 startPoint = inpoint;
				const Trace::Ray lightRay(startPoint, startPoint + lightDir);

				glm::dvec3 poi = glm::dvec3(0);
				glm::dvec3 poiNormal = glm::dvec3(0);
				glm::dvec3 poiColor = glm::dvec3(0);
				bool validInt = false;

				// Check for intersections with other objects.
				for (auto& sceneObject : objlist) {
					// If the object is not the current object.
					if (sceneObject != obj)
						validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor).hit;

					if (validInt) break;
				}

				// If there is no intersection, then we have illumination.
				if (!validInt) {
					const double angle = acos(glm::dot(innormal, lightDir));

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