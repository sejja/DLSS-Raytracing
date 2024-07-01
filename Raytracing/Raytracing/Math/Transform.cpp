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
		mTransform(glm::dmat4(1.f)), mInverse(glm::dmat4(1.f)) {
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Empty Destructor
	*/ // ---------------------------------------------------------------------
	Transform::~Transform() noexcept {}

	// ------------------------------------------------------------------------
	/*! Custom Contructor
	*
	*   Constructs a Transform structure with the given translation, rotation and scale
	*/ // ---------------------------------------------------------------------
	Transform::Transform(const glm::dvec3& translation, 
			const glm::dvec3& rotation, 
			const glm::dvec3& scale) noexcept {
		SetTransform(translation, rotation, scale);
	}

	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*	Builds a Transform with a matrix
	*/ // ---------------------------------------------------------------------
	Transform::Transform(const glm::dmat4& trf) noexcept :
		mTransform(trf), mInverse(glm::inverse(trf)) {
	}

	// ------------------------------------------------------------------------
	/*! Set Transform
	*
	*  Build the transformation matrix with the given translation, rotation and scale
	*/ // ---------------------------------------------------------------------
	void Transform::SetTransform(const glm::dvec3& translation, const glm::dvec3& rotation, const glm::dvec3& scale) noexcept {
		glm::dmat4 trf = glm::translate(glm::dmat4(1.0f), glm::dvec3(translation));
		trf = glm::rotate(trf, rotation.x, glm::dvec3(1.0f, 0.0f, 0.0f));
		trf = glm::rotate(trf, rotation.y, glm::dvec3(0.0f, 1.0f, 0.0f));
		trf = glm::rotate(trf, rotation.z, glm::dvec3(0.0f, 0.0f, 1.0f));
		trf = glm::scale(trf, glm::dvec3(scale));

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