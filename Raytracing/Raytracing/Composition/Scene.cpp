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
#include "../Graphics/Shapes/Plane.h"
#include "../Graphics/Shapes/Sphere.h"
#include "../Graphics/Shapes/Cone.h"
#include "../Graphics/Shapes/Cylinder.h"

namespace Composition {

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Renders the whole Scene into out framebuffer
	*/ // ---------------------------------------------------------------------
	Scene::Scene() {
		auto silverMetal = std::make_shared<Graphics::Materials::MetalicMaterial>();
		auto goldMetal = std::make_shared<Graphics::Materials::MetalicMaterial>();
		auto blueDiffuse = std::make_shared<Graphics::Materials::MetalicMaterial>();
		auto yellowDiffuse = std::make_shared<Graphics::Materials::MetalicMaterial>();
		auto orangeDiffuse = std::make_shared<Graphics::Materials::MetalicMaterial>();
		auto floorMaterial = std::make_shared<Graphics::Materials::MetalicMaterial>();
		auto wallMaterial = std::make_shared<Graphics::Materials::MetalicMaterial>();
		
		silverMetal->SetColor({ 0.5, 0.5, 0.8 });
		silverMetal->SetReflectivity(0.5);
		silverMetal->SetShininess(20.0);

		goldMetal->SetColor({ 0.8, 0.8, 0.3 });
		goldMetal->SetReflectivity(0.25);
		goldMetal->SetShininess(20.0);

		blueDiffuse->SetColor({ 0.2, 0.2, 0.8 });
		blueDiffuse->SetReflectivity(0.05);
		blueDiffuse->SetShininess(5.0);

		yellowDiffuse->SetColor({ 0.8, 0.8, 0.2 });
		yellowDiffuse->SetReflectivity(0.05);
		yellowDiffuse->SetShininess(5.0);

		orangeDiffuse->SetColor({ 1.0, 0.5, 0.0 });
		orangeDiffuse->SetReflectivity(0.05);
		orangeDiffuse->SetShininess(5.0);

		floorMaterial->SetColor({ 1.0, 1.0, 1.0 });
		floorMaterial->SetReflectivity(0.0);
		floorMaterial->SetShininess(0.0);

		wallMaterial->SetColor({ 1.0, 0.125, 0.125 });
		wallMaterial->SetReflectivity(0.75);
		wallMaterial->SetShininess(0.0);


		// Configure the camera.
		mCamera.SetPosition({ 3.0, -5.0, -2.0 });
		mCamera.SetLookAt({ 0.0, 0.0, 0.0 });
		mCamera.SetUp({ 0.0, 0.0, 1.0 });
		mCamera.SetHorizonSize(0.75);
		mCamera.SetAspectRatio(16.0 / 9.0);

		// Create and setup objects.
		auto cone = std::make_shared<Graphics::Shapes::Cone>();
		cone->SetTransform(Math::Transform{ glm::vec3{0.0, 0.0, -0.5},
																							glm::vec3{0.0, 0.0, 0.0},
																							glm::vec3{1.0, 1.0, 2.0} });
		cone->AssignMaterial(silverMetal);

		auto leftSphere = std::make_shared<Graphics::Shapes::Sphere>();
		leftSphere->SetTransform(Math::Transform{ glm::vec3{1.5, -2.0, 0.5},
																										glm::vec3{0.0, 0.0, 0.0},
																										glm::vec3{0.5, 0.5, 0.5} });
		leftSphere->AssignMaterial(blueDiffuse);

		auto rightSphere = std::make_shared<Graphics::Shapes::Sphere>();
		rightSphere->SetTransform(Math::Transform{ glm::vec3{1.5, 0.0, 0.0},
																										glm::vec3{0.0, 0.0, 0.0},
																										glm::vec3{1.0, 1.0, 1.0} });
		rightSphere->AssignMaterial(yellowDiffuse);

		auto topSphere = std::make_shared<Graphics::Shapes::Sphere>();
		topSphere->SetTransform(Math::Transform{ glm::vec3{0.0, 0.0, -1.0},
																										glm::vec3{0.0, 0.0, 0.0},
																										glm::vec3{0.5, 0.5, 0.5} });
		topSphere->AssignMaterial(orangeDiffuse);

		auto floor = std::make_shared<Graphics::Shapes::Plane>();
		floor->SetTransform(Math::Transform{ glm::vec3{0.0, 0.0, 1.0},
																								glm::vec3{0.0, 0.0, 0.0},
																								glm::vec3{16.0, 16.0, 1.0} });
		floor->AssignMaterial(floorMaterial);

		auto leftWall = std::make_shared<Graphics::Shapes::Plane>();
		leftWall->SetTransform(Math::Transform{ glm::vec3{-4.0, 0.0, 0.0},
																									glm::vec3{0.0, -PI / 2.0, -PI / 2.0},
																									glm::vec3{16.0, 16.0, 1.0} });
		leftWall->AssignMaterial(wallMaterial);

		auto backWall = std::make_shared<Graphics::Shapes::Plane>();
		backWall->SetTransform(Math::Transform{ glm::vec3{0.0, 4.0, 0.0},
																									glm::vec3{-PI / 2.0, 0.0, 0.0},
																									glm::vec3{16.0, 16.0, 1.0} });
		backWall->AssignMaterial(wallMaterial);

		auto cylinder1 = std::make_shared<Graphics::Shapes::Cylinder>();
		cylinder1->SetTransform(Math::Transform{ glm::vec3{-1.5, -2.0, 1.0},
																										glm::vec3{0.0, -PI / 2.0, 0.0},
																										glm::vec3{0.25, 0.25, 1.0} });
		cylinder1->AssignMaterial(goldMetal);

		auto cylinder2 = std::make_shared<Graphics::Shapes::Cylinder>();
		cylinder2->SetTransform(Math::Transform{ glm::vec3{-1.0, -2.0, 0.0},
																										glm::vec3{0.0, 0.0, 0.0},
																										glm::vec3{0.25, 0.25, 1.0} });
		cylinder2->AssignMaterial(silverMetal);

		auto cone2 = std::make_shared<Graphics::Shapes::Cone>();
		cone2->SetTransform(Math::Transform{ glm::vec3{0.0, -1.0, 0.0},
																								glm::vec3{PI / 4.0, 0.0, 0.0},
																								glm::vec3{0.5, 0.5, 1.0} });
		cone2->AssignMaterial(goldMetal);

		// Put the objects into the scene.	
		mObjects.push_back(cone);
		mObjects.push_back(leftSphere);
		mObjects.push_back(rightSphere);
		mObjects.push_back(topSphere);
		mObjects.push_back(floor);
		mObjects.push_back(leftWall);
		mObjects.push_back(backWall);
		mObjects.push_back(cylinder1);
		mObjects.push_back(cylinder2);
		mObjects.push_back(cone2);

		mLights.push_back(std::make_shared<Graphics::Primitives::Lighting::PointLight>());
		mLights[0]->SetPosition(glm::dvec3{ 5.0, -10.0, -5.0 });
		mLights[0]->SetColor(glm::dvec3{ 0.f, 0.f, 1.f});

		mLights.push_back(std::make_shared<Graphics::Primitives::Lighting::PointLight>());
		mLights[1]->SetPosition(glm::dvec3{ -5.0, -10.0, -5.0 });
		mLights[1]->SetColor(glm::dvec3{ 1.f, 0.f, 0.f });

		mLights.push_back(std::make_shared<Graphics::Primitives::Lighting::PointLight>());
		mLights[2]->SetPosition(glm::dvec3{ 0.0, -10.0, -5.0 });
		mLights[2]->SetColor(glm::dvec3{ 0.f, 1.f, 0.f });
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
		const double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
		const double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);
		constexpr double minDist = std::numeric_limits<double>::max();
		constexpr double maxDist = std::numeric_limits<double>::min();
		double normX = - 1.0;
		double normY = - 1.0;
		for (size_t y = 0; y < ySize; y++) {
			normY += yFact;
			for (size_t x = 0; x < xSize; x++) {
				// Normalize the x and y coordinates.
				 normX += xFact;

				// Generate the ray for this pixel.
				mCamera.GenerateRay(normX, normY, cameraRay);

				// Test for intersections with all objects in the scene.
				std::shared_ptr<Composition::Object> closestObject = nullptr;
				glm::dvec3 closestIntPoint = glm::dvec3(0.f);
				glm::dvec3 closestLocalNormal = glm::dvec3(0.f);
				glm::dvec3 closestLocalColor = glm::dvec3(0.f);

				/* Compute the illumination for the closest object, assuming that there
					was a valid intersection. */
				if (CastRay(cameraRay, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor)) {
					// Check if the object has a material.
					if (closestObject->HasMaterial()) {
						Graphics::Primitives::Material::mReflectionRayCount = 0;
						// Use the material to compute the color.
						glm::dvec3 matColor = closestObject->GetMaterial()->ComputeColor(mObjects, mLights,
							closestObject, closestIntPoint,
							closestLocalNormal, cameraRay);
						sf::Uint8 red = static_cast<sf::Uint8>(std::min(matColor.x * 255, 255.0));
						sf::Uint8 green = static_cast<sf::Uint8>(std::min(matColor.y * 255, 255.0));
						sf::Uint8 blue = static_cast<sf::Uint8>(std::min(matColor.z * 255, 255.0));
						fb.SetColor(x, y, sf::Color(red, green, blue));
					} else {
						// Use the basic method to compute the color.
						glm::dvec3 matColor = Graphics::Primitives::Material::ComputeColorDiffuse(mObjects, mLights,
							closestObject, closestIntPoint,
							closestLocalNormal, closestObject->GetColor());
						sf::Uint8 red = static_cast<sf::Uint8>(std::min(matColor.x * 255, 255.0));
						sf::Uint8 green = static_cast<sf::Uint8>(std::min(matColor.y * 255, 255.0));
						sf::Uint8 blue = static_cast<sf::Uint8>(std::min(matColor.z * 255, 255.0));
						fb.SetColor(x, y, sf::Color(red, green, blue));
					}
				}
			}
			normX = -1.0;
		}

		return true;
	}

	// ------------------------------------------------------------------------
	/*! Cast Ray
	*
	*   Casts a ray into the scene
	*/ // ---------------------------------------------------------------------
	bool Scene::CastRay(const Trace::Ray& ray, std::shared_ptr<Object>& closestobj, 
									glm::dvec3& inpoint, glm::dvec3& innormal, glm::dvec3& outcolor) noexcept {
		glm::dvec3 closestIntersection = glm::dvec3(0.f);
		glm::dvec3 closestNormal = glm::dvec3(0.f);
		glm::dvec3 closestColor = glm::dvec3(0.f);
		double minDist = std::numeric_limits<double>::max();
		bool foundIntersection = false;

		// Loop over each object in the scene.
		for (auto& obj : mObjects) {

			// Test if we have a valid intersection.
			// If we have a valid intersection, change pixel color to red.
			if (obj->TestIntersection(ray, closestIntersection, closestNormal, closestColor)) {
				foundIntersection = true;

				// Calculate the distance from the camera to the intersection point.
				const double dist = glm::length(closestIntersection - ray.GetOrigin());

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