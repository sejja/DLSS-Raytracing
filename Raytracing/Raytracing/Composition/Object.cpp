//
//	Object.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Object.h"

namespace Composition {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   
	*/ // ---------------------------------------------------------------------
	Object::Object() {

	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   
	*/ // ---------------------------------------------------------------------
	Object::~Object() {

	}

	// ------------------------------------------------------------------------
	/*! Test Intersection
	*
	*   
	*/ // ---------------------------------------------------------------------
	bool Object::TestIntersection(const Trace::Ray& ray, glm::vec3& inpoint, glm::vec3& innormal, glm::vec4& outcolor) {
		return false;
	}

	// ------------------------------------------------------------------------
	/*! Close Enough
	*
	*  
	*/ // ---------------------------------------------------------------------
	bool Object::CloseEnough(const float f1, const float f2) {
		return false;
	}
}