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
		MetalicMaterial::MetalicMaterial() noexcept :
			mColor{ 0.0f }, mShininess(0), mReflectivity(0) {
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
		glm::dvec3 MetalicMaterial::ComputeColor(const std::vector<std::shared_ptr<Composition::Object>>& objList, 
																	const std::vector<std::shared_ptr<Primitives::Lighting::Light>>& lightList, 
																	const std::shared_ptr<Composition::Object>& currObject, 
																	const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint, 
																	const Trace::Ray& camRay, glm::dvec2 uv, int reflectioncount) const noexcept {
			// Define the initial material colors.
			glm::dvec3 matColor = glm::dvec3(0.f);
			glm::dvec3 refColor = glm::dvec3(0.f);
			glm::dvec3 difColor = glm::dvec3(0.f);
			glm::dvec3 spcColor = glm::dvec3(0.f);

			// Compute the diffuse component.
			difColor = ComputeColorDiffuse(objList, lightList, currObject, intersectionPoint, normalPoint, mTexture != nullptr ? mTexture->GetColor(uv) : mColor);

			// Compute the reflection component.
			if (mReflectivity > 0.0)
				refColor = ComputeColorReflection(objList, lightList, currObject, intersectionPoint, normalPoint, camRay, uv, reflectioncount);

			// Combine reflection and diffuse components.
			matColor = (refColor * mReflectivity) + (difColor * (1 - mReflectivity));

			// Compute the specular component.
			if (mShininess > 0.0)
				spcColor = ComputeSpecular(objList, lightList, currObject, intersectionPoint, normalPoint, camRay);

			return matColor + spcColor;
		}

		// ------------------------------------------------------------------------
		/*! Compute Specular
		*
		*   Compute the specular component of the object
		*/ // ---------------------------------------------------------------------
		glm::dvec3 MetalicMaterial::ComputeSpecular(const std::vector<std::shared_ptr<Composition::Object>>& objList,
																		const std::vector<std::shared_ptr<Primitives::Lighting::Light>>& lightList, 
																		const std::shared_ptr<Composition::Object>& currObject, 
																		const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint, 
																		const Trace::Ray& camRay) const noexcept {
			glm::dvec3 col = glm::dvec3();

			// Loop through all of the lights in the scene.
			for (auto& currentLight : lightList) {
				/* Check for intersections with all objects in the scene. */
				double intensity = 0.0;

				// Construct a vector pointing from the intersection point to the light.
				const glm::dvec3 lightDir = glm::normalize(currentLight->GetPosition() - intersectionPoint);

				// Compute a start point.
				const glm::dvec3 startPoint = intersectionPoint + (lightDir * 0.001);

				// Construct a ray from the point of intersection to the light.
				const Trace::Ray lightRay(startPoint, startPoint + lightDir);

				/* Loop through all objects in the scene to check if any
					obstruct light from this source. */
				glm::dvec3 poi = glm::dvec3();;
				glm::dvec3 poiNormal = glm::dvec3();;
				glm::dvec3 poiColor = glm::dvec3();;
				bool validInt = false;
				for (auto& sceneObject : objList) {
					validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor).hit;
					if (validInt)
						break;
				}

				/* If no intersections were found, then proceed with
					computing the specular component. */
				if (!validInt) {
					// Compute the dot product.
					const double dotProduct = glm::dot(glm::reflect(lightRay.GetEndPoint() - lightRay.GetOrigin(), normalPoint),
								glm::normalize(camRay.GetEndPoint() - camRay.GetOrigin()));

					// Only proceed if the dot product is positive.
					if (dotProduct > 0.0f)
						intensity = mReflectivity * std::pow(dotProduct, mShininess);
				}

				col.r += currentLight->GetColor().x * intensity;
				col.g += currentLight->GetColor().y * intensity;
				col.g += currentLight->GetColor().z * intensity;
			}

			return col;
		}

		// ------------------------------------------------------------------------
		/*! Set Color
		*
		*   Sets the Color of an object
		*/ // ---------------------------------------------------------------------
		void MetalicMaterial::SetColor(const glm::dvec3& color) noexcept {
			mColor = color;
		}

		// ------------------------------------------------------------------------
		/*! Set Shininess
		*
		*   Sets the Shininess of the material
		*/ // ---------------------------------------------------------------------
		void MetalicMaterial::SetShininess(double shininess) noexcept {
			mShininess = shininess;
		}

		// ------------------------------------------------------------------------
		/*! 
		*
		*   Compute the specular component of the object
		*/ // ---------------------------------------------------------------------
		void MetalicMaterial::SetReflectivity(double reflectivity) noexcept {
			mReflectivity = reflectivity;
		}
	}
}