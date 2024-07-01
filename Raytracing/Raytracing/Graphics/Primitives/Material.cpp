//
//	Material.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 30/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Material.h"

namespace Graphics {
	namespace Primitives {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*
		*/ // ---------------------------------------------------------------------
		Material::Material() noexcept : mColor{ 0.0f } {}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*
		*/ // ---------------------------------------------------------------------
		Material::~Material() noexcept {}

		// ------------------------------------------------------------------------
		/*! Compute Color
		*
		*
		*/ // ---------------------------------------------------------------------
		glm::dvec3 Material::ComputeColor(
			const std::vector<std::shared_ptr<Composition::Object>>& objList,
			const std::vector<std::shared_ptr<Lighting::Light>>& lightList,
			const std::shared_ptr<Composition::Object>& currObject,
			const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint,
			const Trace::Ray& camRay) const noexcept {
			glm::dvec3 color = mColor;
			
			return color;
		}

		// ------------------------------------------------------------------------
		/*! Compute Color
		*
		*
		*/ // ---------------------------------------------------------------------
		glm::dvec3 Material::ComputeColorDiffuse(
			const std::vector<std::shared_ptr<Composition::Object>>& objList,
			const std::vector<std::shared_ptr<Lighting::Light>>& lightList,
			const std::shared_ptr<Composition::Object>& currObject,
			const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint,
			const glm::dvec3& basecolor) noexcept {
			glm::dvec3 diffuseColor = glm::dvec3(0.f);
			double intensity = 0.0;
			glm::dvec3 color = glm::dvec3(0.f);
			double red = 0.0;
			double green = 0.0;
			double blue = 0.0;
			bool validIllum = false;
			bool illumFound = false;
			for (auto currentLight : lightList)
			{
				validIllum = currentLight->ComputeLighting(intersectionPoint, normalPoint, objList, currObject, color, intensity);
				if (validIllum)
				{
					illumFound = true;
					red += color.x * intensity;
					green += color.y * intensity;
					blue += color.z * intensity;
				}
			}

			if (illumFound)
			{
				diffuseColor.x = red * basecolor.x;
				diffuseColor.y = green * basecolor.y;
				diffuseColor.z = blue * basecolor.z;
			}

			// Return the material color.
			return diffuseColor;
		}

		// ------------------------------------------------------------------------
		/*! Compute Color Reflection
		*
		*   Computes the color reflection
		*/ // ---------------------------------------------------------------------
		glm::dvec3 Material::ComputeColorReflection(const std::vector<std::shared_ptr<Composition::Object>>& objList,
																		const std::vector<std::shared_ptr<Lighting::Light>>& lightList, 
																		const std::shared_ptr<Composition::Object>& currObject, 
																		const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint,
																		const Trace::Ray& camRay) const noexcept {
			glm::dvec3 reflectionColor = glm::dvec3(0.f);
			glm::dvec3 d = camRay.GetEndPoint() - camRay.GetOrigin();
			glm::dvec3 reflectionVector = glm::reflect(d, normalPoint); //MIGHT POTENTIALLY BE WRONG

			Trace::Ray reflectionRay(intersectionPoint, intersectionPoint + reflectionVector);

			std::shared_ptr<Composition::Object> closestObject;
			glm::dvec3 closestinpoint = glm::dvec3(0.f);
			glm::dvec3 closestinnormal = glm::dvec3(0.f);
			glm::dvec3 closestoutcolor = glm::dvec3(0.f);
			bool intersection = CastRay(reflectionRay, objList, closestObject, closestinpoint, closestinnormal, closestoutcolor);

			glm::dvec3 matColor = glm::dvec3();
			if (intersection && mReflectionRayCount < mReflectionCountMax) {
				mReflectionRayCount++;
				if (closestObject->HasMaterial()) {
					matColor = closestObject->GetMaterial()->ComputeColor(objList, lightList, closestObject, closestinpoint, closestinnormal, reflectionRay);
				}
				else {
					matColor = ComputeColorDiffuse(objList, lightList, closestObject, closestinpoint, closestinnormal, closestObject->GetColor());
				}
			}
			else {
				reflectionColor = glm::dvec3(0.0f);
			}

			reflectionColor = matColor;
			return reflectionColor;
		}

		// ------------------------------------------------------------------------
		/*! Cast Ray
		*
		*   Casts a ray into the scene
		*/ // ---------------------------------------------------------------------
		bool Material::CastRay(const Trace::Ray& ray, 
										const std::vector<std::shared_ptr<Composition::Object>>& objList, 
										std::shared_ptr<Composition::Object>& closestobj, glm::dvec3& inpoint, 
										glm::dvec3& innormal, glm::dvec3& outcolor) const noexcept {
			glm::dvec3 intPoint;
			glm::dvec3 localNormal;
			glm::dvec3 localColor;
			double minDist = std::numeric_limits<double>::max();
			bool foundIntersection = false;

			for (auto& obj : objList) {
				if (obj->TestIntersection(ray, intPoint, localNormal, localColor)) {
					foundIntersection = true;
					const double dist = glm::length(intPoint - ray.GetOrigin());
					if (dist < minDist) {
						minDist = dist;
						closestobj = obj;
						inpoint = intPoint;
						innormal = localNormal;
						outcolor = localColor;
					}
				}
			}
			return foundIntersection;
		}
	}
}