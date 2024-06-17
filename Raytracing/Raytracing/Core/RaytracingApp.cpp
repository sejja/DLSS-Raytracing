//
//	RaytracingApp.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 08/06/24
//	Copyright © 2024. All Rights reserved
//

#include "RaytracingApp.h"

namespace Core {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs a RayTracing App
	*/ // ---------------------------------------------------------------------
	RaytracingApp::RaytracingApp()
		: mRunning{ false }, mWindow{ sf::VideoMode(1280, 720), "Raytracing" }, mFrameBuffer({1280, 720}) {}

	// ------------------------------------------------------------------------
	/*! Execute
	*
	*   Called once the Raytracing App is running
	*/ // ---------------------------------------------------------------------
	void RaytracingApp::Execute() {
		//If we failed to initialize the Raytracing App, throw an exception
		if (!Init()) throw RaytracingAppException("Failed to initialize Raytracing App");

		sf::Event event;

		//While the Raytracing App is running, handle events, loop and render
		while (mRunning) {
			//Handle all events
			while (mWindow.pollEvent(event)) Event(event);
			Loop();
			Render();
		}
	}

	// ------------------------------------------------------------------------
	/*! Init
	*
	*   Initializes the Raytracing Window
	*/ // ---------------------------------------------------------------------
	bool RaytracingApp::Init() {
		mRunning = true;
		mFrameBuffer.SetSize(mWindow.getSize().x, mWindow.getSize().y);
		mWindow.clear();
		mScene.Render(mFrameBuffer);
		mFrameBuffer.DrawToRenderTarget(mWindow, sf::RenderStates::Default);
		mWindow.display();
		return mRunning;
	}

	// ------------------------------------------------------------------------
	/*! Event
	*
	*   Handles an SFML event
	*/ // ---------------------------------------------------------------------
	void RaytracingApp::Event(sf::Event& event) noexcept {

		//If we are required to close the window, stop running the program
		if (event.type == sf::Event::Closed) mRunning = false;
	}

	// ------------------------------------------------------------------------
	/*! Loop
	*
	*   Loop function to be executed every frame
	*/ // ---------------------------------------------------------------------
	void RaytracingApp::Loop() {
	}

	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Renders the final image to the viewport
	*/ // ---------------------------------------------------------------------
	void RaytracingApp::Render() {
		//mWindow.clear();
		//mScene.Render(mFrameBuffer);
		//mFrameBuffer.DrawToRenderTarget(mWindow, sf::RenderStates::Default);
		//mWindow.display();
	}
	
	// ------------------------------------------------------------------------
	/*! Exit
	*
	*   Exits the traytracing, releasing all resources
	*/ // ---------------------------------------------------------------------
	void RaytracingApp::Exit() {
		mWindow.close();
	}
}