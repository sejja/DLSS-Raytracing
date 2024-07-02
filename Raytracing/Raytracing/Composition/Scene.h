//
//	Scene.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _SCENE__H_
#define _SCENE__H_

#include <memory>
#include <vector>
#include "../Core/FrameBuffer.h"
#include "../Graphics/Primitives/Camera.h"
#include "../Graphics/Primitives/Lighting/Light.h"

namespace Composition {
	class Scene {
	#pragma region //Constructor
	public:
		Scene();
	#pragma endregion

	#pragma region //Method
		bool Render(Core::FrameBuffer& fb);
		bool CastRay(const Trace::Ray& ray, std::shared_ptr<Object>&closestobj, glm::dvec3& inpoint, 
			glm::dvec3& innormal, glm::dvec3& outcolor) noexcept;
	#pragma endregion

	#pragma region //Members
	private:
		Graphics::Primitives::Camera mCamera;
		std::vector<std::shared_ptr<Composition::Object>> mObjects;
		std::vector<std::shared_ptr<Graphics::Primitives::Lighting::Light>> mLights;
	#pragma endregion
	};
}

#endif