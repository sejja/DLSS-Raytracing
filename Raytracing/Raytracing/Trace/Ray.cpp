//
//	Ray.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 09/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Ray.h"

namespace Trace {
	// ------------------------------------------------------------------------
	/*! Constructor
	*
	*   Constructs a dummy Ray
	*/ // ---------------------------------------------------------------------
	Ray::Ray() noexcept :
		mOrigin(), mEndPoint() {}

	// ------------------------------------------------------------------------
	/*! Constructor
	*
	*   Constructs a Ray with the given origin and endpoint
	*/ // ---------------------------------------------------------------------
	Ray::Ray(const glm::dvec3& origin, const glm::dvec3& endpoint) noexcept
		: mOrigin(origin), mEndPoint(endpoint) {
	}
}