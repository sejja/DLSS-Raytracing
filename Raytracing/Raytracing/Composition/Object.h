//
//	Object.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _OBJECT__H_
#define _OBJECT__H_

#include "../Trace/Ray.h"
#include <glm/glm.hpp>
#include "../Math/Transform.h"
#include "../CommonDefines.h"

namespace Composition {
	class Object {
	#pragma region //Constructors & Destructors
	public:
		Object() noexcept;
		virtual ~Object() noexcept;
	#pragma endregion

	#pragma region //Methods
		inline void SetTransform(const Math::Transform& transform) noexcept;
		DONTDISCARD virtual inline bool TestIntersection(const Trace::Ray& ray, glm::vec3 & inpoint, glm::vec3& innormal, glm::vec4& outcolor) noexcept;
		DONTDISCARD virtual inline bool CloseEnough(const float f1, const float f2) noexcept;
		void inline SetColor(const glm::vec4& color) noexcept;
	#pragma endregion

	#pragma region //Members
	protected:
		glm::vec4 mColor;
		Math::Transform mTransform;
	#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Set Transform
	*
	*   Set Transform in matrix form
	*/ // ---------------------------------------------------------------------
	void Object::SetTransform(const Math::Transform& transform) noexcept {
		mTransform = transform;
	}

	// ------------------------------------------------------------------------
	/*! Set Color
	*
	*   Sets the color of an object
	*/ // ---------------------------------------------------------------------
	void Composition::Object::SetColor(const glm::vec4& color) noexcept {
		mColor = color;
	}

	// ------------------------------------------------------------------------
	/*! Test Intersection
	*
	*
	*/ // ---------------------------------------------------------------------
	bool Object::TestIntersection(const Trace::Ray& ray, glm::vec3& inpoint, glm::vec3& innormal, glm::vec4& outcolor) noexcept {
		return false;
	}

	// ------------------------------------------------------------------------
	/*! Close Enough
	*
	*
	*/ // ---------------------------------------------------------------------
	bool Object::CloseEnough(const float f1, const float f2) noexcept {
		return false;
	}
}

#endif
