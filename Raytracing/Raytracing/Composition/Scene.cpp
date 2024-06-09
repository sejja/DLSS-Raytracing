//
//	Scene.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Scene.h"

namespace Composition {

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Renders the whole Scene into out framebuffer
	*/ // ---------------------------------------------------------------------
	Scene::Scene() {

	}

	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Renders the whole Scene into out framebuffer
	*/ // ---------------------------------------------------------------------
	bool Scene::Render(Core::FrameBuffer& fb) {
		std::size_t sizex = fb.GetWidth();
		std::size_t sizey = fb.GetHeight();

		for (int x = 0; x < sizex; x++)
			for(int y = 0; y < sizey; y++)
				fb.SetColor(x, y,
					sf::Color(static_cast<sf::Uint8>((static_cast<float>(x) / fb.GetWidth()) * 255.f),
						static_cast<sf::Uint8>((static_cast<float>(y) / fb.GetHeight()) * 255.f), 0, 255));

		return true;
	}
}