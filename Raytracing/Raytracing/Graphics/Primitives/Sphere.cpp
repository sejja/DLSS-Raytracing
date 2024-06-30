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
	namespace Primitives {
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
		bool Sphere::TestIntersection(const Trace::Ray& ray, glm::vec3& inpoint, glm::vec3& innormal, glm::vec3& outcolor) noexcept {

			// Transform the ray into the object's space.
			const Trace::Ray newRay = mTransform.InverseTransformRay(ray);

			const float b = 2.f * glm::dot(newRay.GetOrigin(), glm::normalize(newRay.GetEndPoint() - newRay.GetOrigin()));

			// Test whether we actually have an intersection.
			const float intTest = (b * b) - 4.f * (glm::dot(newRay.GetOrigin(), newRay.GetOrigin()) - 1.f);

			// If the discriminant is less than 0, then there is no intersection.
			if (intTest > 0.f) {
				const float numSQRT = sqrtf(intTest);
				const float t1 = (-b + numSQRT) / 2.f;
				const float t2 = (-b - numSQRT) / 2.f;

				/* If either t1 or t2 are negative, then at least part of the object is
					behind the camera and so we will ignore it. */
				if ((t1 < 0.0) || (t2 < 0.0))
					return false;
				else {
					// Determine which point of intersection was closest to the camera.
					inpoint = mTransform.ApplyTransform(newRay.GetOrigin() + (glm::normalize(newRay.GetEndPoint() - newRay.GetOrigin()) * ((t1 < t2) ? t1 : t2)));
					innormal = glm::normalize(inpoint - mTransform.ApplyTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
					outcolor = mColor;
				}

				return true; 
			} else
				return false;
		}
	}
}