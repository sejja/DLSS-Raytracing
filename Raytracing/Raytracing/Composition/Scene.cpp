//
//	Scene.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Scene.h"
#include "../Graphics/Primitives/PointLight.h"
#include <iostream>

namespace Composition {

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Renders the whole Scene into out framebuffer
	*/ // ---------------------------------------------------------------------
	Scene::Scene() {
		// Configure the camera.
		mCamera.SetPosition(glm::vec3{ 0.0, -10.1, 0.0 });
		mCamera.SetLookAt(glm::vec3{ 0.0, 0.0, 0.0 });
		mCamera.SetUp(glm::vec3{ 0.0, 0.0, 1.0 });
		mCamera.SetHorizonSize(0.25);
		mCamera.SetAspectRatio(16.0 / 9.0);
		mObjects.emplace_back(std::make_shared<Graphics::Primitives::Sphere>());
		mLights.push_back(std::make_shared<Graphics::Primitives::PointLight>());
		mLights[0]->mLocation = glm::vec3{ 5.0, -10.0, 5.0 };
		mLights[0]->mColor = glm::vec3{ 255, 255, 255};
	}

	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Renders the whole Scene into out framebuffer
	*/ // ---------------------------------------------------------------------
	bool Scene::Render(Core::FrameBuffer& fb) {
		// Get the dimensions of the output image.
		int xSize = fb.GetWidth();
		int ySize = fb.GetHeight();

		// Loop over each pixel in our image.
		Trace::Ray cameraRay;
		glm::vec3 intPoint;
		glm::vec3 localNormal;
		glm::vec4 localColor;
		float xFact = 1.0 / (static_cast<float>(xSize) / 2.0);
		float yFact = 1.0 / (static_cast<float>(ySize) / 2.0);
		float minDist = 1e6;
		float maxDist = 0.0;
		for (int x = 0; x < xSize; ++x)
		{
			for (int y = 0; y < ySize; ++y)
			{
				// Normalize the x and y coordinates.
				float normX = (static_cast<float>(x) * xFact) - 1.0;
				float normY = (static_cast<float>(y) * yFact) - 1.0;

				// Generate the ray for this pixel.
				mCamera.GenerateRay(normX, normY, cameraRay);

				for (auto& obj : mObjects) {
					// Test if we have a valid intersection.
					bool validInt = obj->TestIntersection(cameraRay, intPoint, localNormal, localColor);

					// If we have a valid intersection, change pixel color to red.
					if (validInt)
					{
						float intensity = 0.0f;
						glm::vec3 color;
						bool validIllumin = false;

						for (auto& light : mLights) {
							validIllumin = light->ComputeLighting(cameraRay, intPoint, localNormal, mObjects, obj, color, intensity);
						}

						// Compute the distance between the camera and the point of intersection.
						double dist = glm::length(intPoint - cameraRay.GetOrigin());
						if (dist > maxDist)
							maxDist = dist;

						if (dist < minDist)
							minDist = dist;

						if(validIllumin)
							fb.SetColor(x, y, { static_cast<sf::Uint8>(255 * intensity),0, 0 });
						else
							fb.SetColor(x, y, { 0, 0, 0 });
					}
					else
					{
						fb.SetColor(x, y, { 0, 0, 0 });
					}	
				}
			}
		}

		std::cout << "Minimum distance: " << minDist << std::endl;
		std::cout << "Maximum distance: " << maxDist << std::endl;

		return true;
	}
}