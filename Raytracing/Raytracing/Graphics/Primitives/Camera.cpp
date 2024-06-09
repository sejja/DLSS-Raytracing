//
//	Camera.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Camera.h"

namespace Graphics {
	namespace Primitives {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Sets the Initial Camera configuration
		*/ // ---------------------------------------------------------------------
		Camera::Camera() :
			mCameraPosition{0.f, -10.f, 0.f }, mCameraLookAt{0.f, 0.f, 0.f }, mCameraUp{0.f, 0.f, 1.f},
			mCameraLength(1.f), mCameraHorizonSize(1.f), mCameraAspectRatio(1.f) {
		}

		// ------------------------------------------------------------------------
		/*! Generate Ray
		*
		*   Generates a Ray from the Camera Position to the given coordinates
		*/ // ---------------------------------------------------------------------
		Trace::Ray Camera::GenerateRay(const float x, const float y) const noexcept {
			glm::vec3 screenWorldPart1 = mProjectionScreenCenter + (mProjectionScreenU * x);
			glm::vec3 screenWorldCoordinate = screenWorldPart1 + (mProjectionScreenV * y);
			
			return Trace::Ray(mCameraPosition, screenWorldCoordinate);
		}

		// ------------------------------------------------------------------------
		/*! Update Camera Geometry
		*
		*   Calculates the Camera Geometry
		*/ // ---------------------------------------------------------------------
		void Camera::UpdateCameraGeometry() {
			mAlignmentVector = glm::normalize(mCameraLookAt - mCameraPosition);
			mProjectionScreenU = glm::normalize(glm::cross(mCameraUp, mAlignmentVector));
			mProjectionScreenV = glm::normalize(glm::cross(mAlignmentVector, mProjectionScreenU));
			mProjectionScreenCenter = mCameraPosition + mCameraLength * mAlignmentVector;
			mProjectionScreenU = mProjectionScreenU * mCameraHorizonSize;
			mProjectionScreenV = mProjectionScreenV * mCameraHorizonSize / mCameraAspectRatio;
		}

		// ------------------------------------------------------------------------
		/*! Set Position
		*
		*   Sets the Camera Position in World Space
		*/ // ---------------------------------------------------------------------
		void Camera::SetPosition(const glm::vec3& position) {
			mCameraPosition = position;
		}

		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Sets the Initial Camera configuration
		*/ // ---------------------------------------------------------------------
		void Camera::SetLookAt(const glm::vec3& direction) {
			mCameraLookAt = direction;
			UpdateCameraGeometry();
		}

		// ------------------------------------------------------------------------
		/*! Set Up
		*
		*   Sets the Up Vector of the Camera
		*/ // ---------------------------------------------------------------------
		void Camera::SetUp(const glm::vec3& up) {
			mCameraUp = up;
			UpdateCameraGeometry();
		}

		// ------------------------------------------------------------------------
		/*! Set Length
		*
		*   Sets the Camera Length
		*/ // ---------------------------------------------------------------------
		void Camera::SetLength(const float length) {
			mCameraLength = length;
			UpdateCameraGeometry();
		}

		// ------------------------------------------------------------------------
		/*! Set Horizon Size
		*
		*   Sets the Camera Horizon Size
		*/ // ---------------------------------------------------------------------
		void Camera::SetHorizonSize(const float size) {
			mCameraHorizonSize = size;
			UpdateCameraGeometry();
		}

		// ------------------------------------------------------------------------
		/*! Set Aspect Ratio
		*
		*   Sets the Camera Aspect Ratio
		*/ // ---------------------------------------------------------------------
		void Camera::SetAspectRatio(const float ratio) {
			mCameraAspectRatio = ratio;
			UpdateCameraGeometry();
		}
	}
}