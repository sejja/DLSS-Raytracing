//
//	Transform.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 24/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Math {
	// ------------------------------------------------------------------------
	/*! Constructor
	*
	*   Sets the default values for the Transform
	*/ // ---------------------------------------------------------------------
	Transform::Transform() noexcept : 
		mTransform(glm::mat4(1.f)), mInverse(glm::mat4(1.f)) {
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Empty Destructor
	*/ // ---------------------------------------------------------------------
	Transform::~Transform() noexcept {}

	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*	Builds a Transform with a matrix
	*/ // ---------------------------------------------------------------------
	Transform::Transform(const glm::mat4& trf) noexcept :
		mTransform(trf), mInverse(glm::inverse(trf)) {
	}

	// ------------------------------------------------------------------------
	/*! Set Transform
	*
	*  Build the transformation matrix with the given translation, rotation and scale
	*/ // ---------------------------------------------------------------------
	void Transform::SetTransform(const glm::vec4& translation, const glm::vec4& rotation, const glm::vec4& scale) noexcept {
		glm::mat4 trf = glm::translate(glm::mat4(1.0f), glm::vec3(translation));
		trf = glm::rotate(trf, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		trf = glm::rotate(trf, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		trf = glm::rotate(trf, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		trf = glm::scale(trf, glm::vec3(scale));

		mTransform = trf;
		mInverse = glm::inverse(trf);
	}

	// ------------------------------------------------------------------------
	/*! Operator=
	*
	*  Asigns a Transform to a new one
	*/ // ---------------------------------------------------------------------
	Transform Transform::operator=(const Transform& rhs) noexcept {
		mTransform = rhs.mTransform;
		mInverse = rhs.mInverse;

		return *this;
	}
}