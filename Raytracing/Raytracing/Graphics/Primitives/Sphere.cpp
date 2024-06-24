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
			// Compute the values of a, b and c.
			glm::vec3 vhat = glm::normalize(ray.GetEndPoint() - ray.GetOrigin());

			/* Note that a is equal to the squared magnitude of the
				direction of the cast ray. As this will be a unit vector,
				we can conclude that the value of 'a' will always be 1. */
				// a = 1.0;

				// Calculate b.
			float b = 2.0 * glm::dot(ray.GetOrigin(), vhat);

			// Calculate c.
			float c = glm::dot(ray.GetOrigin(), ray.GetOrigin()) - 1.0;

			// Test whether we actually have an intersection.
			float intTest = (b * b) - 4.0 * c;

			if (intTest > 0.0)
			{
				float numSQRT = sqrtf(intTest);
				float t1 = (-b + numSQRT) / 2.0;
				float t2 = (-b - numSQRT) / 2.0;

				/* If either t1 or t2 are negative, then at least part of the object is
					behind the camera and so we will ignore it. */
				if ((t1 < 0.0) || (t2 < 0.0))
				{
					return false;
				}
				else
				{
					// Determine which point of intersection was closest to the camera.
					if (t1 < t2)
					{
						inpoint = ray.GetOrigin() + (vhat * t1);
					}
					else
					{
						inpoint = ray.GetOrigin() + (vhat * t2);
					}

					innormal = glm::normalize(inpoint);
				}

				return true;
			}
			else
			{
				return false;
			}
		}
	}
}