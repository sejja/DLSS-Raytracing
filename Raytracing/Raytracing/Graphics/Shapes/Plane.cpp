//
//	Plane.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 25/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Plane.h"

namespace Graphics {
	namespace Shapes {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*  
		*/ // ---------------------------------------------------------------------
		Plane::Plane() noexcept {}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*  
		*/ // ---------------------------------------------------------------------
		Plane::~Plane() noexcept {}

		// ------------------------------------------------------------------------
		/*! Test Intersection
		*
		*   Tests whether a ray intersects witht the plane
		*/ // ---------------------------------------------------------------------
		Trace::Hit Plane::TestIntersection(const Trace::Ray& ray, glm::dvec3& inpoint, glm::dvec3& innormal, glm::dvec3& outcolor) noexcept {
			const Trace::Ray backray = mTransform.InverseTransformRay(ray);

			const glm::dvec3 rayDir = glm::normalize(backray.GetEndPoint() - backray.GetOrigin());

			//If there is an intersection with the plane
			if (!CloseEnough(rayDir.z, 0.f)) {
				const double t = backray.GetOrigin().z / -rayDir.z;

				//If the intersection is in front of the camera
				if (t > 0.f) {
					const double u = backray.GetOrigin().x + (t * rayDir.x);
					const double v = backray.GetOrigin().y + (t * rayDir.y);

					//If the intersection is within the plane
					if ((u >= -1.f && u <= 1.f) && (v >= -1.f && v <= 1.f)) {
						inpoint = mTransform.ApplyTransform(backray.GetOrigin() + (rayDir * t));
						innormal = glm::normalize(mTransform.ApplyTransform(glm::dvec3(0.f, 0.f, -1.f)) - mTransform.ApplyTransform(glm::dvec3(0.0f, 0.0f, 0.0f)));
						outcolor = mColor;
						return Trace::Hit(true, glm::dvec2(u, v));
					}
				}
			}

			return false;
		}
	}
}