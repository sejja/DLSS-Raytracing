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
		*
		*/ // ---------------------------------------------------------------------
		bool Sphere::TestIntersection(const Trace::Ray& ray, glm::vec3& inpoint, glm::vec3& innormal, glm::vec4& outcolor) {
			auto vhat = glm::normalize(ray.GetOrigin() - ray.GetEndPoint());

			/* Note that a is equal to the squared magnitude of the
				direction of the cast ray. As this will be a unit vector,
				we can conclude that the value of 'a' will always be 1. */
				// a = 1.0;

				// Calculate b.
			double b = 2.0 * glm::dot(ray.GetOrigin(), vhat);

			// Calculate c.
			double c = glm::dot(ray.GetOrigin(), ray.GetOrigin()) - 1.0;

			// Test whether we actually have an intersection.
			double intTest = (b * b) - 4.0 * c;

			if (intTest > 0) {
				double numSQRT = sqrt(intTest);
				float t1 = (-b+numSQRT) / 2.f;
				float t2 = (-b - numSQRT) / 2.f;


				 {

					if (t1 < t2) {
						inpoint = ray.GetOrigin() + (vhat * t1);
					}
					else {
						inpoint = ray.GetOrigin() + (vhat * t2);
					}

					return true;
				}
			}
			else {
				return false;
			}
		}
	}
}