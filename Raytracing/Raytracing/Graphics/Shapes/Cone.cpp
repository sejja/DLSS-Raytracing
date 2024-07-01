#include "Cone.h"
#include <array>

namespace Graphics {
	namespace Shapes {
		// The default constructor.
		Cone::Cone()
		{

		}

		// The destructor.
		Cone::~Cone()
		{

		}

		// The function to test for intersections.
		bool Cone::TestIntersection(const Trace::Ray& castRay, glm::dvec3& intPoint,
			glm::dvec3& localNormal, glm::dvec3& localColor) noexcept
		{
			// Copy the ray and apply the backwards transform.
			Trace::Ray bckRay = mTransform.InverseTransformRay(castRay);

			// Copy the m_lab vector from bckRay and normalize it.
			glm::dvec3 v = glm::normalize(bckRay.GetEndPoint() - bckRay.GetOrigin());

			// Get the start point of the line.
			glm::dvec3 p = bckRay.GetOrigin();

			// Compute a, b and c.
			double a = std::pow(v.x, 2.0) + std::pow(v.y, 2.0) - std::pow(v.z, 2.0);
			double b = 2 * (p.x * v.x + p.y * v.y - p.z * v.z);
			double c = std::pow(p.x, 2.0) + std::pow(p.y, 2.0) - std::pow(p.z, 2.0);

			// Compute b^2 - 4ac.
			double numSQRT = sqrt(std::pow(b, 2.0) - 4 * a * c);

			std::array<glm::dvec3, 3> poi = {glm::dvec3(0), glm::dvec3(0), glm::dvec3(0)};
			std::array<double, 3> t = {0, 0, 0};
			bool t1Valid = false;
			bool t2Valid = false;
			bool t3Valid = false;
			if (numSQRT > 0.0)
			{
				// Compute the values of t.
				t.at(0) = (-b + numSQRT) / (2 * a);
				t.at(1) = (-b - numSQRT) / (2 * a);

				// Compute the points of intersection.
				poi.at(0) = bckRay.GetOrigin() + (v * t[0]);
				poi.at(1) = bckRay.GetOrigin() + (v * t[1]);

				if ((t.at(0) > 0.0) && (poi.at(0).z > 0.0) && (poi.at(0).z < 1.0))
				{
					t1Valid = true;
				}
				else
				{
					t1Valid = false;
					t.at(0) = 100e6;
				}

				if ((t.at(1) > 0.0) && (poi.at(1).z > 0.0) && (poi.at(1).z < 1.0))
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

			// And test the end cap.
			if (CloseEnough(v.z, 0.0))
			{
				t3Valid = false;
				t.at(2) = 100e6;
			}
			else
			{
				// Compute values for t.
				t.at(2) = (bckRay.GetOrigin().z - 1.0) / -v.z;

				// Compute points of intersection.
				poi.at(2) = bckRay.GetOrigin() + t.at(2) * v;

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
			}

			// If no valid intersections found, then we can stop.
			if ((!t1Valid) && (!t2Valid) && (!t3Valid))
				return false;

			// Check for the smallest valid value of t.
			int minIndex = 0;
			double minValue = 10e6;
			for (int i = 0; i < 3; ++i)
			{
				if (t.at(i) < minValue)
				{
					minValue = t.at(i);
					minIndex = i;
				}
			}

			/* If minIndex is either 0 or 1, then we have a valid intersection
				with the cone itself. */
			glm::dvec3 validPOI = poi.at(minIndex);
			if (minIndex < 2)
			{
				// Transform the intersection point back into world coordinates.
				intPoint = mTransform.ApplyTransform(validPOI);

				// Compute the local normal.
				glm::dvec3 orgNormal = glm::dvec3(0.f);
				glm::dvec3 newNormal = glm::dvec3(0.f);
				glm::dvec3 localOrigin = glm::dvec3(0.f);
				glm::dvec3 globalOrigin = mTransform.ApplyTransform(localOrigin);
				double tX = validPOI.x;
				double tY = validPOI.y;
				double tZ = -sqrt(pow(tX, 2.0) + pow(tY, 2.0));
				orgNormal.x = tX;
				orgNormal.y = tY;
				orgNormal.z = tZ;
				newNormal = glm::normalize(mTransform.ApplyTransform(orgNormal) - globalOrigin);
				localNormal = newNormal;

				// Return the base color.
				localColor = mColor;

				return true;
			}
			else
			{
				// Check the end cap.
				if (!CloseEnough(v.z, 0.0))
				{
					// Check if we are inside the disk.
					if (sqrt(std::pow(validPOI.x, 2.0) + std::pow(validPOI.y, 2.0)) < 1.0)
					{
						// Transform the intersection point back into world coordinates.
						intPoint = mTransform.ApplyTransform(validPOI);

						// Compute the local normal.
						glm::dvec3 localOrigin{0.0, 0.0, 0.0};
						glm::dvec3 normalVector{ 0.0, 0.0, 1.0};
						glm::dvec3 globalOrigin = mTransform.ApplyTransform(localOrigin);
						localNormal = glm::normalize(mTransform.ApplyTransform(normalVector) - globalOrigin);

						// Return the base color.
						localColor = mColor;

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