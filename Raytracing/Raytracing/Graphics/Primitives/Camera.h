//
//	Camera.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _CAMERA__H_
#define _CAMERA__H_

#include <glm/glm.hpp>
#include "../../Trace/Ray.h"
#include "../../CommonDefines.h"

namespace Graphics {
	namespace Primitives {
		class Camera {
		#pragma region //Constructor
		public:
			Camera();
		#pragma endregion

		#pragma region //Methods
			void SetPosition(const glm::vec3& position);
			void SetLookAt(const glm::vec3& direction);
			void SetUp(const glm::vec3& up);
			void SetLength(const float length);
			void SetHorizonSize(const float size);
			void SetAspectRatio(const float ratio);
			DONTDISCARD inline glm::vec3 GetPosition() const noexcept;
			DONTDISCARD inline glm::vec3 GetLookAt() const noexcept;
			DONTDISCARD inline glm::vec3 GetUp() const noexcept;
			DONTDISCARD inline glm::vec3 GetU() const noexcept;
			DONTDISCARD inline glm::vec3 GetV() const noexcept;
			DONTDISCARD inline glm::vec3 GetScreenCenter() const noexcept;
			DONTDISCARD inline float GetLength() const noexcept;
			DONTDISCARD inline float GetHorizonSize() const noexcept;
			DONTDISCARD inline float GetAspectRatio() const noexcept;
			DONTDISCARD Trace::Ray GenerateRay(const float x, const float y) const noexcept;

		private:
			void UpdateCameraGeometry();
		#pragma endregion

		#pragma region //Members
			glm::vec3 mCameraPosition;
			glm::vec3 mCameraLookAt;
			glm::vec3 mCameraUp;
			float mCameraLength;
			float mCameraHorizonSize;
			float mCameraAspectRatio;
			glm::vec3 mAlignmentVector;
			glm::vec3 mProjectionScreenU;
			glm::vec3 mProjectionScreenV;
			glm::vec3 mProjectionScreenCenter;
		#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get Position
		*
		*   Returns the Camera Position
		*/ // ---------------------------------------------------------------------
		glm::vec3 Camera::GetPosition() const noexcept {
			return mCameraPosition;
		}

		// ------------------------------------------------------------------------
		/*! Get Look At
		*
		*   Returns the Camera Look At
		*/ // ---------------------------------------------------------------------
		glm::vec3 Camera::GetLookAt() const noexcept {
			return mCameraLookAt;
		}

		// ------------------------------------------------------------------------
		/*! Get Up
		*
		*   Returns the Camera Up Vector
		*/ // ---------------------------------------------------------------------
		glm::vec3 Camera::GetUp() const noexcept {
			return mCameraUp;
		}

		// ------------------------------------------------------------------------
		/*! Get U
		*
		*   Returns the Camera U Vector
		*/ // ---------------------------------------------------------------------
		glm::vec3 Camera::GetU() const noexcept {
			return mProjectionScreenU;
		}

		// ------------------------------------------------------------------------
		/*! Get V
		*
		*   Returns the Camera V Vector
		*/ // ---------------------------------------------------------------------
		glm::vec3 Camera::GetV() const noexcept {
			return mProjectionScreenV;
		}

		// ------------------------------------------------------------------------
		/*! Get Screen Center
		*
		*   Returns the Camera Screen Center
		*/ // ---------------------------------------------------------------------
		glm::vec3 Camera::GetScreenCenter() const noexcept {
			return mProjectionScreenCenter;
		}

		// ------------------------------------------------------------------------
		/*! Get Length
		*
		*   Get the Camera Length
		*/ // ---------------------------------------------------------------------
		float Camera::GetLength() const noexcept {
			return mCameraLength;
		}

		// ------------------------------------------------------------------------
		/*! Get Horizon Size
		*
		*   Get the Camera Horizon Size
		*/ // ---------------------------------------------------------------------
		float Camera::GetHorizonSize() const noexcept {
			return mCameraHorizonSize;
		}

		// ------------------------------------------------------------------------
		/*! Get Aspect Ratio
		*
		*   Returns the Camera Aspect Ratio
		*/ // ---------------------------------------------------------------------
		float Camera::GetAspectRatio() const noexcept {
			return mCameraAspectRatio;
		}
	}
}

#endif