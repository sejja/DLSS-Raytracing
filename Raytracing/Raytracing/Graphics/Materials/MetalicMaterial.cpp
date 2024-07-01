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
			mReflectionCountMax = 1;
			mShininess = 0.f;
			mReflectivity = 0.f;
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
		glm::dvec3 MetalicMaterial::ComputeColor(const std::vector<std::shared_ptr<Composition::Object>>& objList, 
																	const std::vector<std::shared_ptr<Primitives::Lighting::Light>>& lightList, 
																	const std::shared_ptr<Composition::Object>& currObject, 
																	const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint, 
																	const Trace::Ray& camRay) const noexcept {
			// Define the initial material colors.
			glm::dvec3 matColor = glm::dvec3(0.f);
			glm::dvec3 refColor = glm::dvec3(0.f);
			glm::dvec3 difColor = glm::dvec3(0.f);
			glm::dvec3 spcColor = glm::dvec3(0.f);

			// Compute the diffuse component.
			difColor = ComputeColorDiffuse(objList, lightList, currObject, intersectionPoint, normalPoint, mColor);

			// Compute the reflection component.
			if (mReflectivity > 0.0)
				refColor = ComputeColorReflection(objList, lightList, currObject, intersectionPoint, normalPoint, camRay);

			// Combine reflection and diffuse components.
			matColor = (refColor * mReflectivity) + (difColor * (1 - mReflectivity));

			// Compute the specular component.
			if (mShininess > 0.0)
				spcColor = ComputeSpecular(objList, lightList, currObject, intersectionPoint, normalPoint, camRay);

			// Add the specular component to the final color.
			matColor = matColor + spcColor;

			return matColor;
		}

		// ------------------------------------------------------------------------
		/*! Compute Specular
		*
		*
		*/ // ---------------------------------------------------------------------
		glm::dvec3 MetalicMaterial::ComputeSpecular(const std::vector<std::shared_ptr<Composition::Object>>& objList,
																		const std::vector<std::shared_ptr<Primitives::Lighting::Light>>& lightList, 
																		const std::shared_ptr<Composition::Object>& currObject, 
																		const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint, 
																		const Trace::Ray& camRay) const noexcept {
			glm::dvec3 spcColor = glm::dvec3();
			double red = 0.0;
			double green = 0.0;
			double blue = 0.0;

			// Loop through all of the lights in the scene.
			for (auto currentLight : lightList)
			{
				/* Check for intersections with all objects in the scene. */
				double intensity = 0.0;

				// Construct a vector pointing from the intersection point to the light.
				glm::dvec3 lightDir = glm::normalize(currentLight->GetPosition() - intersectionPoint);

				// Compute a start point.
				glm::dvec3 startPoint = intersectionPoint + (lightDir * 0.001);

				// Construct a ray from the point of intersection to the light.
				Trace::Ray lightRay(startPoint, startPoint + lightDir);

				/* Loop through all objects in the scene to check if any
					obstruct light from this source. */
				glm::dvec3 poi = glm::dvec3();;
				glm::dvec3 poiNormal = glm::dvec3();;
				glm::dvec3 poiColor = glm::dvec3();;
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
					glm::dvec3 d = lightRay.GetEndPoint() - lightRay.GetOrigin();
					glm::dvec3 r = glm::normalize(d - (2 * glm::dot(d, normalPoint) * normalPoint));

					// Compute the dot product.
					glm::dvec3 v = glm::normalize(camRay.GetEndPoint() - camRay.GetOrigin());
					double dotProduct = glm::dot(r, v);

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
		void MetalicMaterial::SetColor(const glm::dvec3& color) noexcept {
			mColor = color;
		}
		void MetalicMaterial::SetShininess(double shininess) noexcept {
			mShininess = shininess;
		}
		void MetalicMaterial::SetReflectivity(double reflectivity) noexcept {
			mReflectivity = reflectivity;
		}
	}
}