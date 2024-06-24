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
		namespace Lighting {
			// ------------------------------------------------------------------------
			/*! Constructor
			*
			*   Constructs a Light source
			*/ // ---------------------------------------------------------------------
			Light::Light() :
				mColor(), mPosition(), mIntensity(0.0f) {}

			// ------------------------------------------------------------------------
			/*! Destructor
			*
			*   Destroys the Light
			*/ // ---------------------------------------------------------------------
			Light::~Light() {}
		}
	}
}