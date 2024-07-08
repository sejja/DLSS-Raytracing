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
		*   Compute the color of the object
		*/ // ---------------------------------------------------------------------
		glm::dvec3 Material::ComputeColor(
			const std::vector<std::shared_ptr<Composition::Object>>& objList,
			const std::vector<std::shared_ptr<Lighting::Light>>& lightList,
			const std::shared_ptr<Composition::Object>& currObject,
			const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint,
			const Trace::Ray& camRay) const noexcept {
			return mColor;
		}

		// ------------------------------------------------------------------------
		/*! Compute Color
		*
		*	Compute the Diffuse channel of the color
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
			glm::dvec3 tempcol = glm::dvec3(0.f);
			bool illumFound = false;

			// Compute the diffuse color.
			for (auto& currentLight : lightList)

				// Compute the light intensity.
				if (currentLight->ComputeLighting(intersectionPoint, normalPoint, objList, currObject, color, intensity)) {
					illumFound = true;
					tempcol.r += color.x * intensity;
					tempcol.g += color.y * intensity;
					tempcol.b += color.z * intensity;
				}

			// If we found an illumination, compute the final color.
			if (illumFound) {
				diffuseColor.x = tempcol.r * basecolor.x;
				diffuseColor.y = tempcol.g * basecolor.y;
				diffuseColor.z = tempcol.b * basecolor.z;
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
			const Trace::Ray reflectionRay(intersectionPoint, intersectionPoint + glm::reflect(camRay.GetEndPoint() - camRay.GetOrigin(), normalPoint));

			std::shared_ptr<Composition::Object> closestObject = nullptr;
			glm::dvec3 closestinpoint = glm::dvec3(0);
			glm::dvec3 closestinnormal = glm::dvec3(0);
			glm::dvec3 closestoutcolor = glm::dvec3(0);
			glm::dvec3 matColor = glm::dvec3(0);

			// Cast the reflection ray.
			if (CastRay(reflectionRay, objList, closestObject, closestinpoint, closestinnormal, closestoutcolor)
				&& mReflectionRayCount < mReflectionCountMax) {
				mReflectionRayCount++;
				
				// Check if the closest object has a material.
				if (closestObject->HasMaterial())
					matColor = closestObject->GetMaterial()->ComputeColor(objList, lightList, closestObject, closestinpoint, closestinnormal, reflectionRay);
				else
					matColor = ComputeColorDiffuse(objList, lightList, closestObject, closestinpoint, closestinnormal, closestObject->GetColor());
			}

			return matColor;
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
			glm::dvec3 intPoint = glm::dvec3(0);
			glm::dvec3 localNormal = glm::dvec3(0);
			glm::dvec3 localColor = glm::dvec3(0);
			double minDist = std::numeric_limits<double>::max();
			bool foundIntersection = false;

			// Loop through all of the objects in the scene.
			for (auto& obj : objList)
				// Check for intersections with the current object.
				if (obj->TestIntersection(ray, intPoint, localNormal, localColor)) {
					foundIntersection = true;
					const double dist = glm::length(intPoint - ray.GetOrigin());
					// Check if the intersection is the closest one.
					if (dist < minDist) {
						minDist = dist;
						closestobj = obj;
						inpoint = intPoint;
						innormal = localNormal;
						outcolor = localColor;
					}
				}

			return foundIntersection;
		}

		void Material::SetTexture(const std::shared_ptr<Texture>& texture) noexcept {
			mTexture = texture;
		}
	}
}