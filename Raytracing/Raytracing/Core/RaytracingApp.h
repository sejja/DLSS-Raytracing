//
//	RaytracingApp.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 08/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _RAYTRACING__APP_H_
#define _RAYTRACING__APP_H_

#include <SFML/Graphics.hpp>
#include "FrameBuffer.h"
#include "../CommonDefines.h"
#include "../Composition/Scene.h"

namespace Core {
	class RaytracingApp {
	#pragma region //Declarations
		CLASS_EXCEPTION(RaytracingApp)
	#pragma endregion

	#pragma region //Constructors & Destructors
	public:
		RaytracingApp();
	#pragma endregion

	#pragma region //Methods
		void Execute();
		bool Init();
		void Event(sf::Event& event) noexcept;
		void Loop();
		void Render();
		void Exit();
	#pragma endregion

	#pragma region //Members
	private:
		bool mRunning;
		sf::RenderWindow mWindow;
		FrameBuffer mFrameBuffer;
		Composition::Scene mScene;
	#pragma endregion
	};
}

#endif