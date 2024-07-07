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

namespace Graphics {
	namespace Primitives {
		class Material;
	}
}

namespace Composition {
	class Object {
	#pragma region //Constructors & Destructors
	public:
		Object() noexcept;
		virtual ~Object() noexcept;
	#pragma endregion

	#pragma region //Methods
		inline void SetTransform(const Math::Transform& transform) noexcept;
		DONTDISCARD virtual inline bool TestIntersection(const Trace::Ray& ray, glm::dvec3 & inpoint, glm::dvec3& innormal, glm::dvec3& outcolor) noexcept;
		DONTDISCARD virtual inline bool CloseEnough(const double f1, const double f2) noexcept;
		void inline SetColor(const glm::dvec3& color) noexcept;
		bool AssignMaterial(const std::shared_ptr<Graphics::Primitives::Material>& objMaterial) noexcept;
		DONTDISCARD inline bool HasMaterial() const noexcept;
		DONTDISCARD inline std::shared_ptr<Graphics::Primitives::Material> GetMaterial() const noexcept;
		DONTDISCARD inline glm::dvec3 GetColor() const noexcept;
		DONTDISCARD inline glm::dvec2 GetUV() const noexcept;
	#pragma endregion

	#pragma region //Members
	protected:
		glm::dvec3 mColor;
		Math::Transform mTransform;
		std::shared_ptr<Graphics::Primitives::Material> mMaterial;
		glm::dvec2 mUVs;
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
	void Composition::Object::SetColor(const glm::dvec3& color) noexcept {
		mColor = color;
	}

	// ------------------------------------------------------------------------
	/*! Test Intersection
	*
	*
	*/ // ---------------------------------------------------------------------
	bool Object::TestIntersection(const Trace::Ray& ray, glm::dvec3& inpoint, glm::dvec3& innormal, glm::dvec3& outcolor) noexcept {
		return false;
	}

	// ------------------------------------------------------------------------
	/*! Close Enough
	*
	*
	*/ // ---------------------------------------------------------------------
	bool Object::CloseEnough(const double f1, const double f2) noexcept {
		return fabs(f1 - f2) < std::numeric_limits<double>::epsilon();
	}

	// ------------------------------------------------------------------------
	/*! Has Material
	*
	*	Returns wether an object has a material or not
	*/ // ---------------------------------------------------------------------
	bool Object::HasMaterial() const noexcept {
		return  static_cast<bool>(mMaterial);
	}
	
	// ------------------------------------------------------------------------
	/*! Get Material
	*
	*	Returns the material of the object
	*/ // ---------------------------------------------------------------------
	std::shared_ptr<Graphics::Primitives::Material> Object::GetMaterial() const noexcept {
		return mMaterial;
	}

	// ------------------------------------------------------------------------
	/*! Get Color
	*
	*	Returns the Color of the object
	*/ // ---------------------------------------------------------------------
	glm::dvec3 Object::GetColor() const noexcept {
		return mColor;
	}

	glm::dvec2 Object::GetUV() const noexcept
	{
		return mUVs;
	}
}

#endif
