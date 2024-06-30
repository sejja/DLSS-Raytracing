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
		glm::vec3 Material::ComputeColor(
			const std::vector<std::shared_ptr<Composition::Object>>& objList,
			const std::vector<std::shared_ptr<Lighting::Light>>& lightList,
			const std::shared_ptr<Composition::Object>& currObject,
			const glm::vec3& intersectionPoint, const glm::vec3& normalPoint,
			const Trace::Ray& camRay) const noexcept {
			glm::vec3 color = mColor;
			
			return color;
		}

		// ------------------------------------------------------------------------
		/*! Compute Color
		*
		*
		*/ // ---------------------------------------------------------------------
		glm::vec3 Material::ComputeColorDiffuse(
			const std::vector<std::shared_ptr<Composition::Object>>& objList,
			const std::vector<std::shared_ptr<Lighting::Light>>& lightList,
			const std::shared_ptr<Composition::Object>& currObject,
			const glm::vec3& intersectionPoint, const glm::vec3& normalPoint,
			const glm::vec3& basecolor) noexcept {
			glm::vec3 diffuseColor;
			float intensity;
			glm::vec3 color;
			float red = 0.0;
			float green = 0.0;
			float blue = 0.0;
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
		glm::vec3 Material::ComputeColorReflection(const std::vector<std::shared_ptr<Composition::Object>>& objList,
																		const std::vector<std::shared_ptr<Lighting::Light>>& lightList, 
																		const std::shared_ptr<Composition::Object>& currObject, 
																		const glm::vec3& intersectionPoint, const glm::vec3& normalPoint,
																		const Trace::Ray& camRay) const noexcept {
			glm::vec3 reflectionColor;
			glm::vec3 d = camRay.GetEndPoint() - camRay.GetOrigin();
			glm::vec3 reflectionVector = glm::reflect(d, normalPoint); //MIGHT POTENTIALLY BE WRONG

			Trace::Ray reflectionRay(intersectionPoint, intersectionPoint + reflectionVector);

			std::shared_ptr<Composition::Object> closestObject;
			glm::vec3 closestinpoint;
			glm::vec3 closestinnormal;
			glm::vec3 closestoutcolor;
			bool intersection = CastRay(reflectionRay, objList, closestObject, closestinpoint, closestinnormal, closestoutcolor);

			glm::vec3 matColor;
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
				reflectionColor = glm::vec3(0.0f);
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
										std::shared_ptr<Composition::Object>& closestobj, glm::vec3& inpoint, 
										glm::vec3& innormal, glm::vec3& outcolor) const noexcept {
			glm::vec3 intPoint;
			glm::vec3 localNormal;
			glm::vec3 localColor;
			float minDist = std::numeric_limits<float>::max();
			bool foundIntersection = false;

			for (auto& obj : objList) {
				if (obj->TestIntersection(ray, intPoint, localNormal, localColor)) {
					foundIntersection = true;
					const float dist = glm::length(intPoint - ray.GetOrigin());
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