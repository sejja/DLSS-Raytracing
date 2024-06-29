//
//	Scene.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Scene.h"
#include "../Graphics/Primitives/Lighting/PointLight.h"

namespace Composition {

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Renders the whole Scene into out framebuffer
	*/ // ---------------------------------------------------------------------
	Scene::Scene() {
		// Configure the camera.
		mCamera.SetPosition(glm::vec3{ 0.0, -10, -1.f });
		mCamera.SetLookAt(glm::vec3{ 0.0, 0.0, 0.0 });
		mCamera.SetUp(glm::vec3{ 0.0, 0.0, 1.0 });
		mCamera.SetHorizonSize(0.25);
		mCamera.SetAspectRatio(16.f / 9.f);
		mObjects.emplace_back(std::make_shared<Graphics::Primitives::Sphere>());
		mObjects.emplace_back(std::make_shared<Graphics::Primitives::Sphere>());
		mObjects.emplace_back(std::make_shared<Graphics::Primitives::Sphere>());
		mObjects.emplace_back(std::make_shared<Graphics::Primitives::Plane>());

		Math::Transform t1;
		t1.SetTransform(glm::vec4(-1.5f, 0.f, 0.f, 1.f), 
								glm::vec4(0.f, 0.f, 0.f, 1.f), 
								glm::vec4(0.5f, 0.5f, 0.5f, 1.f));

		Math::Transform t2;
		t2.SetTransform(glm::vec4(0.f, 0.f, 0.f, 1.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f),
			glm::vec4(0.75f, 0.5f, 0.5f, 1.f));

		Math::Transform t3;
		t3.SetTransform(glm::vec4(1.5f, 0.f, 0.f, 1.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f),
			glm::vec4(0.75f, 0.75f, 1.f, 1.f));

		Math::Transform t4;
		t4.SetTransform(glm::vec4(0.f, 0.f, 0.75f, 1.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f),
			glm::vec4(4.f, 4.f, 1.f, 1.f));

		mObjects[0]->SetTransform(t1);
		mObjects[1]->SetTransform(t2);
		mObjects[2]->SetTransform(t3);
		mObjects[3]->SetTransform(t4);

		mObjects[0]->SetColor(glm::vec4(1.f, 0.0, 0.0, 1.f));
		mObjects[1]->SetColor(glm::vec4(0.0, 1.f, 0.0, 1.f));
		mObjects[2]->SetColor(glm::vec4 (0.0, 0.0, 1.f, 1.f));
		mObjects[3]->SetColor(glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));

		mLights.push_back(std::make_shared<Graphics::Primitives::Lighting::PointLight>());
		mLights[0]->SetPosition(glm::vec3{ 5.0, -10.0, -5.0 });
		mLights[0]->SetColor(glm::vec3{ 0.f, 0.f, 1.f});

		mLights.push_back(std::make_shared<Graphics::Primitives::Lighting::PointLight>());
		mLights[1]->SetPosition(glm::vec3{ -5.0, -10.0, -5.0 });
		mLights[1]->SetColor(glm::vec3{ 1.f, 0.f, 0.f });

		mLights.push_back(std::make_shared<Graphics::Primitives::Lighting::PointLight>());
		mLights[2]->SetPosition(glm::vec3{ 0.0, -10.0, -5.0 });
		mLights[2]->SetColor(glm::vec3{ 0.f, 1.f, 0.f });
	}

	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Renders the whole Scene into out framebuffer
	*/ // ---------------------------------------------------------------------
	bool Scene::Render(Core::FrameBuffer& fb) {
		// Get the dimensions of the output image.
		const size_t xSize = fb.GetWidth();
		const size_t ySize = fb.GetHeight();

		// Loop over each pixel in our image.
		Trace::Ray cameraRay;
		glm::vec3 intPoint;
		glm::vec3 localNormal;
		glm::vec4 localColor;
		const float xFact = 1.f / (static_cast<float>(xSize) / 2.f);
		const float yFact = 1.f / (static_cast<float>(ySize) / 2.f);

		// Loop over each pixel in our image.
		for (size_t x = 0; x < xSize; ++x) {

			// Loop over each pixel in our image.
			for (size_t y = 0; y < ySize; ++y) {
				// Normalize the x and y coordinates.
				const float normX = (static_cast<float>(x) * xFact) - 1.f;
				const float normY = (static_cast<float>(y) * yFact) - 1.f;

				// Generate the ray for this pixel.
				mCamera.GenerateRay(normX, normY, cameraRay);

				std::shared_ptr<Composition::Object> closestObject = nullptr;
				glm::vec3 closestIntersection = glm::vec3(0.0f);
				glm::vec3 closestNormal = glm::vec3(0.0f);
				glm::vec4 closestColor = glm::vec4(0.0f);
				float minDist = std::numeric_limits<float>::max();
				bool foundIntersection = false;

				// Loop over each object in the scene.
				for (auto& obj : mObjects) {

					// Test if we have a valid intersection.
					// If we have a valid intersection, change pixel color to red.
					if (obj->TestIntersection(cameraRay, intPoint, localNormal, localColor)) {
						foundIntersection = true;

						// Calculate the distance from the camera to the intersection point.
						const float dist = glm::length(intPoint - cameraRay.GetOrigin());

						// If this is the closest object, store the intersection point and color.
						if (dist < minDist) {
							minDist = dist;
							closestIntersection = intPoint;
							closestNormal = localNormal;
							closestColor = localColor;
							closestObject = obj;
						}
					}	
				}

				// If we found an intersection, set the pixel color to the object's color.
				if (foundIntersection) {
					// Calculate the lighting for this pixel.
					float intensity = 0.0f;
					glm::vec3 color;
					float r = 0, g = 0, b = 0;
					bool validIlluminate = false;
					bool ilimationfound = false;
					for (auto& light : mLights) {
						validIlluminate = light->ComputeLighting(closestIntersection, closestNormal, mObjects, closestObject, color, intensity);
						
						if (validIlluminate) {
							ilimationfound = true;
							r += color.x * intensity;
							g += color.y * intensity;
							b += color.z * intensity;
						}
					}

					if (ilimationfound) {
						r *= closestColor.x;
						g *= closestColor.y;
						b *= closestColor.z;
						fb.SetColor(x, y, sf::Color{ static_cast<sf::Uint8>(r * 255),
																static_cast<sf::Uint8>(g * 255),
																static_cast<sf::Uint8>(b* 255), 255 });
					}
				}
			}
		}


		return true;
	}
}