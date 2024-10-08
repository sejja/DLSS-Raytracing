//
//	Sphere.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 9/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _SPHERE__H_
#define _SPHERE__H_

#include "../../Composition/Object.h"
#include "../../Math/Transform.h"

namespace Graphics {
	namespace Shapes {
		class Sphere : public Composition::Object {
		#pragma region //Constructors & Destructors
		public:
			Sphere();
			virtual ~Sphere() override;
		#pragma endregion

		#pragma region //Methods
			Trace::Hit TestIntersection(const Trace::Ray& ray, glm::dvec3& inpoint, glm::dvec3& innormal, glm::dvec3& outcolor) noexcept override;
		#pragma endregion
		};
	}
}

#endif