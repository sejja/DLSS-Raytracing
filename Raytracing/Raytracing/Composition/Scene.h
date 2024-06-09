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

namespace Composition {
	class Scene {
	public:
		Scene();
		bool Render(Core::FrameBuffer& fb);
	};
}

#endif