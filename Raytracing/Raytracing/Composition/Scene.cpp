//
//	Scene.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Scene.h"
#include <iostream>

namespace Composition {

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Renders the whole Scene into out framebuffer
	*/ // ---------------------------------------------------------------------
	Scene::Scene() {
		mCamera.SetPosition(glm::vec3(0, -10.f, 0));
		mCamera.SetLookAt(glm::vec3(0, 0, 0));
		mCamera.SetUp(glm::vec3(0, 0, 1));
		mCamera.SetHorizonSize(1.f);
		mCamera.SetAspectRatio(16.f / 9.f);
	}

	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Renders the whole Scene into out framebuffer
	*/ // ---------------------------------------------------------------------
	bool Scene::Render(Core::FrameBuffer& fb) {
		// Get the dimensions of the output image.
		size_t xSize = fb.GetWidth();
		size_t ySize = fb.GetHeight();

		// Loop over each pixel in our image.
		Trace::Ray cameraRay;
		glm::vec3 intPoint;
		glm::vec3 localNormal;
		glm::vec4 localColor;
		double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
		double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);
		double minDist = 1e6;
		double maxDist = 0.0;
		for (int x = 0; x < xSize; ++x)
		{
			for (int y = 0; y < ySize; ++y)
			{
				// Normalize the x and y coordinates.
				double normX = (static_cast<double>(x) * xFact) - 1.0;
				double normY = (static_cast<double>(y) * yFact) - 1.0;

				// Generate the ray for this pixel.
				mCamera.GenerateRay(normX, normY, cameraRay);

				// Test if we have a valid intersection.
				bool validInt = mSphere.TestIntersection(cameraRay, intPoint, localNormal, localColor);

				// If we have a valid intersection, change pixel color to red.
				if (validInt) {
					// Compute the distance between the camera and the point of intersection.
					float dist = glm::length(intPoint - cameraRay.GetOrigin());
					if (dist > maxDist)
						maxDist = dist;

					if (dist < minDist)
						minDist = dist;

					fb.SetColor(x, y, sf::Color(static_cast<sf::Uint8>(((dist) / 0.94605) * 255), 0, 0));
				} else{
					fb.SetColor(x, y, {0, 0, 0, 255});
				}
			}
		}

		return true;
	}
}