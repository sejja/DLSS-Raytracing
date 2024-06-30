//
//	MetalicMaterial.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 30/06/24
//	Copyright © 2024. All Rights reserved
//

#include "MetalicMaterial.h"

namespace Graphics {
	namespace Materials {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*
		*/ // ---------------------------------------------------------------------
		MetalicMaterial::MetalicMaterial() noexcept : mColor{ 0.0f } {
			mReflectionRayCount = 0;
			mReflectionCountMax = 3;
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*
		*/ // ---------------------------------------------------------------------
		MetalicMaterial::~MetalicMaterial() noexcept {}

		// ------------------------------------------------------------------------
		/*! Compute Color
		*
		*   Compute the color of the object
		*/ // ---------------------------------------------------------------------
		glm::vec3 MetalicMaterial::ComputeColor(const std::vector<std::shared_ptr<Composition::Object>>& objList, 
																	const std::vector<std::shared_ptr<Primitives::Lighting::Light>>& lightList, 
																	const std::shared_ptr<Composition::Object>& currObject, 
																	const glm::vec3& intersectionPoint, const glm::vec3& normalPoint, 
																	const Trace::Ray& camRay) const noexcept {
			// Define the initial material colors.
			glm::vec3 matColor = glm::vec3(0.f);
			glm::vec3 refColor = glm::vec3(0.f);
			glm::vec3 difColor = glm::vec3(0.f);
			glm::vec3 spcColor = glm::vec3(0.f);

			// Compute the diffuse component.
			difColor = ComputeColorDiffuse(objList, lightList, currObject, intersectionPoint, normalPoint, mColor);

			// Compute the reflection component.
			if (mReflectivity > 0.f)
				refColor = ComputeColorReflection(objList, lightList, currObject, intersectionPoint, normalPoint, camRay);

			// Compute the specular component.
			if (mShininess > 0.f)
				spcColor = ComputeSpecular(objList, lightList, currObject, intersectionPoint, normalPoint, camRay);

			// Add the specular component to the final color.
			matColor = matColor + spcColor + difColor + refColor;

			return matColor;
		}

		// ------------------------------------------------------------------------
		/*! Compute Specular
		*
		*
		*/ // ---------------------------------------------------------------------
		glm::vec3 MetalicMaterial::ComputeSpecular(const std::vector<std::shared_ptr<Composition::Object>>& objList,
																		const std::vector<std::shared_ptr<Primitives::Lighting::Light>>& lightList, 
																		const std::shared_ptr<Composition::Object>& currObject, 
																		const glm::vec3& intersectionPoint, const glm::vec3& normalPoint, 
																		const Trace::Ray& camRay) const noexcept {
			glm::vec3 spcColor;
			float red = 0.0;
			float green = 0.0;
			float blue = 0.0;

			// Loop through all of the lights in the scene.
			for (auto currentLight : lightList)
			{
				/* Check for intersections with all objects in the scene. */
				float intensity = 0.0;

				// Construct a vector pointing from the intersection point to the light.
				glm::vec3 lightDir = glm::normalize(currentLight->GetPosition() - intersectionPoint);

				// Compute a start point.
				glm::vec3 startPoint = intersectionPoint + (lightDir * 0.001f);

				// Construct a ray from the point of intersection to the light.
				Trace::Ray lightRay(startPoint, startPoint + lightDir);

				/* Loop through all objects in the scene to check if any
					obstruct light from this source. */
				glm::vec3 poi;
				glm::vec3 poiNormal;
				glm::vec3 poiColor;
				bool validInt = false;
				for (auto sceneObject : objList)
				{
					validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);
					if (validInt)
						break;
				}

				/* If no intersections were found, then proceed with
					computing the specular component. */
				if (!validInt)
				{
					// Compute the reflection vector.
					glm::vec3 d = lightRay.GetEndPoint() - lightRay.GetOrigin();
					glm::vec3 r = glm::normalize(d - (2 * glm::dot(d, normalPoint) * normalPoint));

					// Compute the dot product.
					glm::vec3 v = glm::normalize(camRay.GetEndPoint() - camRay.GetOrigin());
					float dotProduct = glm::dot(r, v);

					// Only proceed if the dot product is positive.
					if (dotProduct > 0.0f)
					{
						intensity = mReflectivity * std::pow(dotProduct, mShininess);
					}
				}

				red += currentLight->GetColor().x * intensity;
				green += currentLight->GetColor().y * intensity;
				blue += currentLight->GetColor().z * intensity;
			}

			spcColor.x = red;
			spcColor.y = green;
			spcColor.z = blue;
			return spcColor;
		}
		void MetalicMaterial::SetColor(const glm::vec3& color) noexcept {
			mColor = color;
		}
		void MetalicMaterial::SetShininess(float shininess) noexcept {
			mShininess = shininess;
		}
		void MetalicMaterial::SetReflectivity(float reflectivity) noexcept {
			mReflectivity = reflectivity;
		}
	}
}