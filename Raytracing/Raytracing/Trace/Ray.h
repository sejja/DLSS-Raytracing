//
//	Ray.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 09/06/24
//	Copyright � 2024. All Rights reserved
//

#ifndef _RAY__H_
#define _RAY__H_

#include <glm/glm.hpp>
#include "../CommonDefines.h"

namespace Trace {
	class Ray {
#pragma region //Constructor
	public:
		Ray() noexcept;
		Ray(const glm::vec3& origin, const glm::vec3& endpoint) noexcept;
#pragma endregion

#pragma region //Methods
		inline void SetOrigin(const glm::vec3& origin) noexcept;
		inline void SetEndPoint(const glm::vec3& endpoint) noexcept;
		DONTDISCARD inline glm::vec3 GetOrigin() const noexcept;
		DONTDISCARD inline glm::vec3 GetEndPoint() const noexcept;
#pragma endregion

#pragma endregion //Members
	private:
		glm::vec3 mOrigin, mEndPoint;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Get Origin
	*
	*   Gets the Origin Point of the Ray
	*/ // ---------------------------------------------------------------------
	inline void Ray::SetOrigin(const glm::vec3& origin) noexcept {
		mOrigin = origin;
	}

	// ------------------------------------------------------------------------
	/*! Get Origin
	*
	*   Gets the Origin Point of the Ray
	*/ // ---------------------------------------------------------------------
	inline void Ray::SetEndPoint(const glm::vec3& endpoint) noexcept {
		mEndPoint = endpoint;
	}

	// ------------------------------------------------------------------------
	/*! Get Origin
	*
	*   Gets the Origin Point of the Ray
	*/ // ---------------------------------------------------------------------
	glm::vec3 Ray::GetOrigin() const noexcept {
		return mOrigin;
	}

	// ------------------------------------------------------------------------
	/*! Get End Point
	*
	*   Gets the Ray Endpoint
	*/ // ---------------------------------------------------------------------
	glm::vec3 Ray::GetEndPoint() const noexcept {
		return mEndPoint;
	}
}

#endif