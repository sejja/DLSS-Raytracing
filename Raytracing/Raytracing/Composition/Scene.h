//
//	Scene.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _SCENE__H_
#define _SCENE__H_

#include "../Core/FrameBuffer.h"
#include "../Graphics/Primitives/Sphere.h"
#include "../Graphics/Primitives/Camera.h"

namespace Composition {
	class Scene {
	#pragma region //Constructor
	public:
		Scene();
	#pragma endregion

	#pragma region //Method
		bool Render(Core::FrameBuffer& fb);
	#pragma endregion

	#pragma region //Members
	private:
		Graphics::Primitives::Camera mCamera;
		Graphics::Primitives::Sphere mSphere;
	#pragma endregion
	};
}

#endif