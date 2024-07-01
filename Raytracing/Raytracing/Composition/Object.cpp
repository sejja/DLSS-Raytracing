//
//	Object.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Object.h"
#include "../Graphics/Primitives/Material.h"

namespace Composition {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   
	*/ // ---------------------------------------------------------------------
	Object::Object() noexcept {}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   
	*/ // ---------------------------------------------------------------------
	Object::~Object() noexcept {}


	// ------------------------------------------------------------------------
	/*! Assign Material
	*
	*
	*/ // ---------------------------------------------------------------------
	bool Object::AssignMaterial(const std::shared_ptr<Graphics::Primitives::Material>& objMaterial) noexcept {
		mMaterial = objMaterial;
		mHasMaterial = static_cast<bool>(objMaterial);
		return mHasMaterial;
	}
}