//
//	Scene.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Scene.h"
#include "../Graphics/Primitives/Lighting/PointLight.h"
#include "../Graphics/Primitives/Material.h"
#include "../Graphics/Materials/MetalicMaterial.h"

namespace Composition {

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Renders the whole Scene into out framebuffer
	*/ // ---------------------------------------------------------------------
	Scene::Scene() {
		auto testMat = std::make_shared<Graphics::Materials::MetalicMaterial>();
		testMat->SetColor(glm::vec3(0.25f, 0.5f, 0.8f));
		testMat->SetReflectivity(0.5f);
		testMat->SetShininess(10.f);

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
			glm::vec4(0.5f, 0.5f, 0.5f, 1.f));

		Math::Transform t3;
		t3.SetTransform(glm::vec4(1.5f, 0.f, 0.f, 1.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f),
			glm::vec4(0.5f, 0.5f, 0.5f, 1.f));

		Math::Transform t4;
		t4.SetTransform(glm::vec4(0.f, 0.f, 0.75f, 1.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f),
			glm::vec4(4.f, 4.f, 1.f, 1.f));

		mObjects[0]->SetTransform(t1);
		mObjects[1]->SetTransform(t2);
		mObjects[2]->SetTransform(t3);
		mObjects[3]->SetTransform(t4);

		mObjects[0]->SetColor(glm::vec3(1.f, 0.0, 0.0));
		mObjects[1]->SetColor(glm::vec3(0.0, 1.f, 0.0));
		mObjects[2]->SetColor(glm::vec3 (0.0, 0.0, 1.f));
		mObjects[3]->SetColor(glm::vec3(0.5f, 0.5f, 0.5f));

		mObjects[0]->AssignMaterial(testMat);

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
		int xSize = fb.GetWidth();
		int ySize = fb.GetHeight();

		// Loop over each pixel in our image.
		Trace::Ray cameraRay;
		glm::vec3 intPoint;
		glm::vec3 localNormal;
		glm::vec3 localColor;
		double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
		double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);
		double minDist = 1e6;
		double maxDist = 0.0;
		for (int y = 0; y < ySize; ++y)
		{
			for (int x = 0; x < xSize; ++x)
			{
				// Normalize the x and y coordinates.
				double normX = (static_cast<double>(x) * xFact) - 1.0;
				double normY = (static_cast<double>(y) * yFact) - 1.0;

				// Generate the ray for this pixel.
				mCamera.GenerateRay(normX, normY, cameraRay);

				// Test for intersections with all objects in the scene.
				std::shared_ptr<Composition::Object> closestObject;
				glm::vec3 closestIntPoint;
				glm::vec3 closestLocalNormal;
				glm::vec3 closestLocalColor;
				bool intersectionFound = CastRay(cameraRay, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);

				/* Compute the illumination for the closest object, assuming that there
					was a valid intersection. */
				if (intersectionFound)
				{
					// Check if the object has a material.
					if (closestObject->HasMaterial()) {
						Graphics::Primitives::Material::mReflectionRayCount = 0;
						// Use the material to compute the color.
						glm::vec3 matColor = closestObject->GetMaterial()->ComputeColor(mObjects, mLights,
							closestObject, closestIntPoint,
							closestLocalNormal, cameraRay);
						fb.SetColor(x, y, sf::Color(matColor.x * 255, matColor.y * 255, matColor.z * 255, 1));
					}
					else
					{
						// Use the basic method to compute the color.
						glm::vec3 matColor = Graphics::Primitives::Material::ComputeColorDiffuse(mObjects, mLights,
							closestObject, closestIntPoint,
							closestLocalNormal, closestObject->GetColor());
						fb.SetColor(x, y, sf::Color(matColor.x * 255, matColor.y * 255, matColor.z * 255, 1));
					}
				}
			}
		}

		return true;
	}

	// ------------------------------------------------------------------------
	/*! Cast Ray
	*
	*   Casts a ray into the scene
	*/ // ---------------------------------------------------------------------
	bool Scene::CastRay(const Trace::Ray& ray, std::shared_ptr<Object>& closestobj, 
									glm::vec3& inpoint, glm::vec3& innormal, glm::vec3& outcolor) {
		glm::vec3 closestIntersection;
		glm::vec3 closestNormal;
		glm::vec3 closestColor;
		float minDist = std::numeric_limits<float>::max();
		bool foundIntersection = false;

		// Loop over each object in the scene.
		for (auto& obj : mObjects) {

			// Test if we have a valid intersection.
			// If we have a valid intersection, change pixel color to red.
			if (obj->TestIntersection(ray, closestIntersection, closestNormal, closestColor)) {
				foundIntersection = true;

				// Calculate the distance from the camera to the intersection point.
				const float dist = glm::length(closestIntersection - ray.GetOrigin());

				// If this is the closest object, store the intersection point and color.
				if (dist < minDist) {
					minDist = dist;
					inpoint = closestIntersection;
					innormal = closestNormal;
					outcolor = closestColor;
					closestobj = obj;
				}
			}
		}

		return foundIntersection;
	}
}