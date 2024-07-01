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
			void SetPosition(const glm::dvec3& position) noexcept;
			void SetLookAt(const glm::dvec3& direction) noexcept;
			void SetUp(const glm::dvec3& up) noexcept;
			void SetLength(const double length) noexcept;
			void SetHorizonSize(const double size) noexcept;
			void SetAspectRatio(const double ratio) noexcept;
			DONTDISCARD inline glm::dvec3 GetPosition() const noexcept;
			DONTDISCARD inline glm::dvec3 GetLookAt() const noexcept;
			DONTDISCARD inline glm::dvec3 GetUp() const noexcept;
			DONTDISCARD inline glm::dvec3 GetU() const noexcept;
			DONTDISCARD inline glm::dvec3 GetV() const noexcept;
			DONTDISCARD inline glm::dvec3 GetScreenCenter() const noexcept;
			DONTDISCARD inline double GetLength() const noexcept;
			DONTDISCARD inline double GetHorizonSize() const noexcept;
			DONTDISCARD inline double GetAspectRatio() const noexcept;
			bool GenerateRay(const double x, const double y, Trace::Ray& cameraRay) const noexcept;

		private:
			void UpdateCameraGeometry() noexcept;
		#pragma endregion
		
		#pragma region //Members
			glm::dvec3 mCameraPosition;
			glm::dvec3 mCameraLookAt;
			glm::dvec3 mCameraUp;
			double mCameraLength;
			double mCameraHorizonSize;
			double mCameraAspectRatio;
			glm::dvec3 mAlignmentVector;
			glm::dvec3 mProjectionScreenU;
			glm::dvec3 mProjectionScreenV;
			glm::dvec3 mProjectionScreenCenter;
		#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get Position
		*
		*   Returns the Camera Position
		*/ // ---------------------------------------------------------------------
		glm::dvec3 Camera::GetPosition() const noexcept {
			return mCameraPosition;
		}

		// ------------------------------------------------------------------------
		/*! Get Look At
		*
		*   Returns the Camera Look At
		*/ // ---------------------------------------------------------------------
		glm::dvec3 Camera::GetLookAt() const noexcept {
			return mCameraLookAt;
		}

		// ------------------------------------------------------------------------
		/*! Get Up
		*
		*   Returns the Camera Up Vector
		*/ // ---------------------------------------------------------------------
		glm::dvec3 Camera::GetUp() const noexcept {
			return mCameraUp;
		}

		// ------------------------------------------------------------------------
		/*! Get U
		*
		*   Returns the Camera U Vector
		*/ // ---------------------------------------------------------------------
		glm::dvec3 Camera::GetU() const noexcept {
			return mProjectionScreenU;
		}

		// ------------------------------------------------------------------------
		/*! Get V
		*
		*   Returns the Camera V Vector
		*/ // ---------------------------------------------------------------------
		glm::dvec3 Camera::GetV() const noexcept {
			return mProjectionScreenV;
		}

		// ------------------------------------------------------------------------
		/*! Get Screen Center
		*
		*   Returns the Camera Screen Center
		*/ // ---------------------------------------------------------------------
		glm::dvec3 Camera::GetScreenCenter() const noexcept {
			return mProjectionScreenCenter;
		}

		// ------------------------------------------------------------------------
		/*! Get Length
		*
		*   Get the Camera Length
		*/ // ---------------------------------------------------------------------
		double Camera::GetLength() const noexcept {
			return mCameraLength;
		}

		// ------------------------------------------------------------------------
		/*! Get Horizon Size
		*
		*   Get the Camera Horizon Size
		*/ // ---------------------------------------------------------------------
		double Camera::GetHorizonSize() const noexcept {
			return mCameraHorizonSize;
		}

		// ------------------------------------------------------------------------
		/*! Get Aspect Ratio
		*
		*   Returns the Camera Aspect Ratio
		*/ // ---------------------------------------------------------------------
		double Camera::GetAspectRatio() const noexcept {
			return mCameraAspectRatio;
		}
	}
}

#endif