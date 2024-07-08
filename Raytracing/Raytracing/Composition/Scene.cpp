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
#include "../Graphics/Textures/Checker.h"
#include "../Graphics/Textures/Sprite.h"

namespace Composition {

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Renders the whole Scene into out framebuffer
	*/ // ---------------------------------------------------------------------
	Scene::Scene() {
		auto testMat = std::make_shared<Graphics::Materials::MetalicMaterial>();
		testMat->SetColor(glm::dvec3(0.25f, 0.5f, 0.8f));
		testMat->SetReflectivity(0.5f);
		testMat->SetShininess(10.f);

		auto testMat2 = std::make_shared<Graphics::Materials::MetalicMaterial>();
		testMat2->SetColor(glm::dvec3(0.25f, 0.5f, 0.8f));
		testMat2->SetReflectivity(0.5f);
		testMat2->SetShininess(10.f);

		auto testMat3 = std::make_shared<Graphics::Materials::MetalicMaterial>();
		testMat3->SetColor(glm::dvec3(1.f, 1.f, 1.f));
		testMat3->SetReflectivity(0.f);
		testMat3->SetShininess(0.f);

		auto checker_text = std::make_shared<Graphics::Textures::Checker>();
		checker_text->SetTransform({ 0.0, 0.0, 1.0 }, 0.0, { 18.0, 18.0, 1.0 });

		auto prite_text = std::make_shared<Graphics::Textures::Sprite>("../Content/mario.png");
		prite_text->SetTransform({ 2.0, 0.0, 1.0 }, 0.0, { 1.0, 1.0, 1.0 });

		// Configure the camera.
		mCamera.SetPosition({ 3.0, -5.0, -2.0 });
		mCamera.SetLookAt({ 0.0, 0.0, 0.0 });
		mCamera.SetUp({ 0.0, 0.0, 1.0 });
		mCamera.SetHorizonSize(0.75);
		mCamera.SetAspectRatio(16.0 / 9.0);

		mObjects.emplace_back(std::make_shared<Graphics::Shapes::Cone>());
		mObjects.emplace_back(std::make_shared<Graphics::Shapes::Sphere>());
		mObjects.emplace_back(std::make_shared<Graphics::Shapes::Cylinder>());
		mObjects.emplace_back(std::make_shared<Graphics::Shapes::Plane>());
		//mObjects.emplace_back(std::make_shared<Graphics::Shapes::Plane>());

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
			glm::vec4(18.f, 8.f, 1.f, 1.f));

		auto leftWall = std::make_shared<Graphics::Shapes::Plane>();
		leftWall->SetTransform(Math::Transform{ glm::vec3{-4.0, 0.0, 0.0},
																glm::vec3{0.0, -PI / 2.0, -PI / 2.0},
																									glm::vec3{16.0, 16.0, 1.0} });

		auto backWall = std::make_shared<Graphics::Shapes::Plane>();
		backWall->SetTransform(Math::Transform{ glm::vec3{0.0, 4.0, 0.0},
																								glm::vec3{-PI / 2.0, 0.0, 0.0},
																								glm::vec3{36.0, 16.0, 1.0} });

		//mObjects.emplace_back(leftWall);
		mObjects.emplace_back(backWall);
		backWall->SetColor({ 0.25f, 0.25f, 0.6f });
		leftWall->SetColor({ 0.6f, 0.25f, 0.25f });
		//backWall->AssignMaterial(testMat);

		mObjects[0]->SetTransform(t1);
		mObjects[1]->SetTransform(t2);
		mObjects[2]->SetTransform(t3);
		mObjects[3]->SetTransform(t4);
		//mObjects[3]->SetTransform(t4);

		mObjects[0]->SetColor(glm::dvec3(1.f, 0.0, 0.0));
		mObjects[1]->SetColor(glm::dvec3(0.0, 1.f, 0.0));
		mObjects[2]->SetColor(glm::dvec3(0.0, 0.0, 1.f));
		mObjects[3]->SetColor(glm::dvec3(0.5f, 0.5f, 0.5f));

		mObjects[1]->AssignMaterial(testMat2);
		mObjects[3]->AssignMaterial(testMat);
		mObjects[2]->AssignMaterial(testMat3);
		mObjects[3]->GetMaterial()->SetTexture(checker_text);
		mObjects[2]->GetMaterial()->SetTexture(prite_text);

		mLights.push_back(std::make_shared<Graphics::Primitives::Lighting::PointLight>());
		mLights[0]->SetPosition(glm::dvec3{ 5.0, -10.0, -5.0 });
		mLights[0]->SetColor(glm::dvec3{ 0.f, 0.f, 1.f });

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