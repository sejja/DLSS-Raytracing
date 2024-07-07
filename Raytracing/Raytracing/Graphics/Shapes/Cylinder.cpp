#include "Cylinder.h"
#include <array>

namespace Graphics {
	namespace Shapes {
		Cylinder::Cylinder()
		{

		}

		// The destructor.
		Cylinder::~Cylinder()
		{

		}

		// The function to test for intersections.
		bool Cylinder::TestIntersection(const Trace::Ray& castRay, glm::dvec3& intPoint,
			glm::dvec3& localNormal, glm::dvec3& localColor) noexcept
		{
			// Copy the ray and apply the backwards transform.
			Trace::Ray bckRay = mTransform.InverseTransformRay(castRay);

			// Copy the m_lab vector from bckRay and normalize it.
			glm::dvec3 v = glm::normalize(bckRay.GetEndPoint() - bckRay.GetOrigin());

			// Get the start point of the line.
			glm::dvec3 p = bckRay.GetOrigin();

			// Compute a, b and c.
			double a = std::pow(v.x, 2.0) + std::pow(v.y, 2.0);
			double b = 2.0 * (p.x * v.x + p.y * v.y);
			double c = std::pow(p.x, 2.0) + std::pow(p.y, 2.0) - 1.0;

			// Compute b^2 - 4ac.
			double numSQRT = sqrt(std::pow(b, 2.0) - 4 * a * c);

			// Test for intersections.
			// First with the cylinder itself.
			std::array<glm::dvec3, 4> poi = {glm::dvec3(0), glm::dvec3(0), glm::dvec3(0), glm::dvec3(0)};
			std::array<double, 4> t = {0, 0, 0, 0};
			bool t1Valid = false;
			bool t2Valid = false;
			bool t3Valid = false;
			bool t4Valid = false;
			if (numSQRT > 0.0)
			{
				// There was an intersection.
				// Compute the values for t.
				t.at(0) = (-b + numSQRT) / (2 * a);
				t.at(1) = (-b - numSQRT) / (2 * a);

				// Compute the points of intersection.
				poi.at(0) = bckRay.GetOrigin() + (v * t[0]);
				poi.at(1) = bckRay.GetOrigin() + (v * t[1]);

				// Check if any of these are valid.
				if ((t.at(0) > 0.0) && (fabs(poi.at(0).z) < 1.0))
				{
					t1Valid = true;
				}
				else
				{
					t1Valid = false;
					t.at(0) = 100e6;
				}

				if ((t.at(1) > 0.0) && (fabs(poi.at(1).z) < 1.0))
				{
					t2Valid = true;
				}
				else
				{
					t2Valid = false;
					t.at(1) = 100e6;
				}
			}
			else
			{
				t1Valid = false;
				t2Valid = false;
				t.at(0) = 100e6;
				t.at(1) = 100e6;
			}

			// And test the end caps.
			if (CloseEnough(v.z, 0.0))
			{
				t3Valid = false;
				t4Valid = false;
				t.at(2) = 100e6;
				t.at(3) = 100e6;
			}
			else
			{
				// Compute the values of t.
				t.at(2) = (bckRay.GetOrigin().z - 1.0) / -v.z;
				t.at(3) = (bckRay.GetOrigin().z + 1.0) / -v.z;

				// Compute the points of intersection.
				poi.at(2) = bckRay.GetOrigin() + t.at(2) * v;
				poi.at(3) = bckRay.GetOrigin() + t.at(3) * v;

				// Check if these are valid.
				if ((t.at(2) > 0.0) && (sqrt(std::pow(poi.at(2).x, 2.0) + std::pow(poi.at(2).y, 2.0)) < 1.0))
				{
					t3Valid = true;
				}
				else
				{
					t3Valid = false;
					t.at(2) = 100e6;
				}

				if ((t.at(3) > 0.0) && (sqrt(std::pow(poi.at(3).x, 2.0) + std::pow(poi.at(3).y, 2.0)) < 1.0))
				{
					t4Valid = true;
				}
				else
				{
					t4Valid = false;
					t.at(3) = 100e6;
				}
			}

			// If no valid intersections found, the we can stop.
			if ((!t1Valid) && (!t2Valid) && (!t3Valid) && (!t4Valid))
				return false;

			// Check for the smallest valid value of t.
			int minIndex = 0;
			double minValue = 10e6;
			for (int i = 0; i < 4; ++i)
			{
				if (t.at(i) < minValue)
				{
					minValue = t.at(i);
					minIndex = i;
				}
			}

			/* If minIndex is either 0 or 1, then we have a valid intersection
				with the cylinder itself. */
			glm::dvec3 validPOI = poi.at(minIndex);
			if (minIndex < 2)
			{
				// Transform the intersection point back into world coordinates.
				intPoint = mTransform.ApplyTransform(validPOI);

				// Compute the local normal.
				glm::dvec3 orgNormal = glm::dvec3(0);
				glm::dvec3 newNormal = glm::dvec3(0);
				glm::dvec3 localOrigin = glm::dvec3(0);
				glm::dvec3 globalOrigin = mTransform.ApplyTransform(localOrigin);
				orgNormal.x = validPOI.x;
				orgNormal.y = validPOI.y;
				orgNormal.z = 0.f;
				newNormal = glm::normalize(mTransform.ApplyTransform(orgNormal) - globalOrigin);
				localNormal = newNormal;

				// Return the base color.
				localColor = mColor;
				double x = validPOI.x;
				double y = validPOI.y;
				double z = validPOI.z;
				double u = atan2(y, x) / PI;
				double v = z;

				mUVs = glm::dvec2(u, v);

				return true;
			}
			else
			{
				// Otherwise check the end caps.
				if (!CloseEnough(v.z, 0.0))
				{
					// Check if we are inside the disk.
					if (sqrt(std::pow(validPOI.x, 2.0) + std::pow(validPOI.y, 2.0)) < 1.0)
					{
						// Transform the intersection point back into world coordinates.
						intPoint = mTransform.ApplyTransform(validPOI);

						// Compute the local normal.
						glm::dvec3 localOrigin{0.0, 0.0, 0.0};
						glm::dvec3 normalVector{ 0.0, 0.0, 0.0 + validPOI.z };
						glm::dvec3 globalOrigin = mTransform.ApplyTransform(localOrigin);
						localNormal = glm::normalize(mTransform.ApplyTransform(normalVector) - globalOrigin);

						// Return the base color.
						localColor = mColor;
						double x = validPOI.x;
						double y = validPOI.y;
						double z = validPOI.z;

						mUVs = glm::dvec2(x, y);

						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}

			return false;
		}
	}
}