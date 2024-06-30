//
//	Plane.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 25/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _PLANE__H_
#define _PLANE__H_

#include <glm/glm.hpp>
#include "../../Composition/Object.h"

namespace Graphics {
	namespace Primitives {
		class Plane : public Composition::Object {
#pragma region //Constructors & Destructors
		public:
			Plane() noexcept;
			virtual ~Plane() noexcept;
#pragma endregion

#pragma region //Methods
			bool TestIntersection(const Trace::Ray& ray, glm::vec3& inpoint, glm::vec3& innormal, glm::vec3& outcolor) noexcept override;
#pragma endregion

#pragma region //Members
		protected:
#pragma endregion
		};
	}

}

#endif