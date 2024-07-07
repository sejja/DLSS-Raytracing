//
//	Sphere.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Sphere.h"
#include <iostream>

namespace Graphics {
	namespace Shapes {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*
		*/ // ---------------------------------------------------------------------
		Sphere::Sphere() {

		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*
		*/ // ---------------------------------------------------------------------
		Sphere::~Sphere() {

		}

		// ------------------------------------------------------------------------
		/*! Test Intersection
		*
		*   Tests whether a ray intersects with the sphere.
		*/ // ---------------------------------------------------------------------
		bool Sphere::TestIntersection(const Trace::Ray& ray, glm::dvec3& inpoint, glm::dvec3& innormal, glm::dvec3& outcolor) noexcept {

			// Transform the ray into the object's space.
			const Trace::Ray newRay = mTransform.InverseTransformRay(ray);

			const double b = 2.f * glm::dot(newRay.GetOrigin(), glm::normalize(newRay.GetEndPoint() - newRay.GetOrigin()));

			// Test whether we actually have an intersection.
			const double intTest = (b * b) - 4.f * (glm::dot(newRay.GetOrigin(), newRay.GetOrigin()) - 1.f);

			// If the discriminant is less than 0, then there is no intersection.
			if (intTest > 0.f) {
				const double numSQRT = sqrt(intTest);
				const double t1 = (-b + numSQRT) / 2.f;
				const double t2 = (-b - numSQRT) / 2.f;

				/* If either t1 or t2 are negative, then at least part of the object is
					behind the camera and so we will ignore it. */
				if ((t1 < 0.0) || (t2 < 0.0))
					return false;
				else {
					// Determine which point of intersection was closest to the camera.
					inpoint = mTransform.ApplyTransform(newRay.GetOrigin() + (glm::normalize(newRay.GetEndPoint() - newRay.GetOrigin()) * ((t1 < t2) ? t1 : t2)));
					innormal = glm::normalize(inpoint - mTransform.ApplyTransform(glm::dvec3(0.0f, 0.0f, 0.0f)));
					outcolor = mColor;

					double x = inpoint.x;
					double y = inpoint.y;
					double z = inpoint.z;
					double u = atan(sqrt(x * x + y * y) / z);
					double v = atan(y / x);

					if (x < 0.0)
						v += PI;

					u /= PI;
					v /= PI;

					mUVs = glm::dvec2(u, v);
				}

				return true; 
			} else
				return false;
		}
	}
}