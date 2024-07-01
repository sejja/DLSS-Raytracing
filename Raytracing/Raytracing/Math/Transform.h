//
//	Transform.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 24/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _TRANSFORM__H_
#define _TRANSFORM__H_

#include <glm/glm.hpp>
#include "../Trace/Ray.h"

namespace Math {
	class Transform {
#pragma region //Constructors & Destructors
	public:
		Transform() noexcept;
		~Transform() noexcept;

		Transform(const glm::dvec3& translation, const glm::dvec3& rotation, const glm::dvec3& scale) noexcept;
		Transform(const glm::dmat4& trf) noexcept;
#pragma endregion

#pragma region //Methods
		void SetTransform(const glm::dvec3& translation, const glm::dvec3& rotation, const glm::dvec3& scale) noexcept;

		DONTDISCARD inline glm::dmat4 GetForward() const noexcept;
		DONTDISCARD inline glm::dmat4 GetInverse() const noexcept;
		DONTDISCARD inline Trace::Ray TransformRay(const Trace::Ray& ray) const noexcept;
		DONTDISCARD inline glm::dvec3 ApplyTransform(const glm::dvec3& vec) const  noexcept;
		DONTDISCARD inline Trace::Ray InverseTransformRay(const Trace::Ray& ray) const noexcept;
		DONTDISCARD inline glm::dvec3 InverseApplyTransform(const glm::dvec3& vec) const  noexcept;
		Transform operator=( const Transform& rhs) noexcept;
#pragma endregion

#pragma region //Members
	private:
		glm::dmat4 mTransform;
		glm::dmat4 mInverse;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Get Forward
	*
	*   Returns the forward transformation matrix
	*/ // ---------------------------------------------------------------------
	glm::dmat4 Transform::GetForward() const noexcept {
		return mTransform;
	}

	// ------------------------------------------------------------------------
	/*! Get Inverse
	*
	*   Returns the inverse transformation matrix
	*/ // ---------------------------------------------------------------------
	glm::dmat4 Transform::GetInverse() const noexcept {
		return mInverse;
	}

	// ------------------------------------------------------------------------
	/*! Transform Ray
	*
	*  Applies the Transformation to a Ray
	*/ // ---------------------------------------------------------------------
	Trace::Ray Transform::TransformRay(const Trace::Ray& ray) const noexcept {
		return Trace::Ray(ApplyTransform(ray.GetOrigin()), ApplyTransform(ray.GetEndPoint()));
	}

	// ------------------------------------------------------------------------
	/*! Apply Transform
	*
	*  Applies the Transformation to a vector
	*/ // ---------------------------------------------------------------------
	glm::dvec3 Transform::ApplyTransform(const glm::dvec3& vec) const noexcept {
		return glm::dvec3(mTransform * glm::vec4(vec, 1.0f));
	}

	// ------------------------------------------------------------------------
	/*! Invserse Transform Ray
	*
	*  Applies the Inverse Transformation to a Ray
	*/ // ---------------------------------------------------------------------
	Trace::Ray Transform::InverseTransformRay(const Trace::Ray& ray) const noexcept {
		return Trace::Ray(InverseApplyTransform(ray.GetOrigin()), InverseApplyTransform(ray.GetEndPoint()));
	}

	// ------------------------------------------------------------------------
	/*! Inverse Apply Transform
	*
	*  Applies the Transformation to a vector
	*/ // ---------------------------------------------------------------------
	glm::dvec3 Transform::InverseApplyTransform(const glm::dvec3& vec) const noexcept {
		return glm::dvec3(mInverse * glm::vec4(vec, 1.0f));
	}
}

#endif