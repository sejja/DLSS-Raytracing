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

namespace Composition {
	class Object {
	#pragma region //Constructors & Destructors
	public:
		Object();
		virtual ~Object();
	#pragma endregion

	#pragma region //Methods
		virtual bool TestIntersection(const Trace::Ray& ray, glm::vec3 & inpoint, glm::vec3& innormal, glm::vec4& outcolor);
		virtual bool CloseEnough(const float f1, const float f2);
	#pragma endregion

	#pragma region //Members
	private:
		glm::vec4 mColor;
	#pragma endregion
	};
}

#endif
