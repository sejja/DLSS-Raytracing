//
//	Light.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 24/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Light.h"

namespace Graphics {
	namespace Primitives {
		// ------------------------------------------------------------------------
		/*! Constructor
		*
		*   Constructs a Light source
		*/ // ---------------------------------------------------------------------
		Light::Light() :
			mColor(), mLocation(), mIntensity(0.0f) {}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Destroys the Light
		*/ // ---------------------------------------------------------------------
		Light::~Light() {}

		// ------------------------------------------------------------------------
		/*! ComputeLighting
		*
		*   Computes the lighting for the given ray
		*/ // ---------------------------------------------------------------------
		bool Light::ComputeLighting(const Trace::Ray& ray, const glm::vec3& inpoint, const glm::vec3& innormal,
			const std::vector<std::shared_ptr<Composition::Object>>& objlist,
			const std::shared_ptr<Composition::Object>& obj,
			glm::vec3& color, float& intensity) {
			return false;
		}	
	}
}