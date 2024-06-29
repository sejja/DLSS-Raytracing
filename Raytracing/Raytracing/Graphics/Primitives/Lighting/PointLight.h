//
//	PointLight.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 24/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _POINT_LIGHT__H_
#define _POINT_LIGHT__H_

#include "Light.h"

namespace Graphics {
	namespace Primitives {
		namespace Lighting {
			class PointLight : public Light {
#pragma region //Constructors & Destructors
			public:
				PointLight() noexcept;
				virtual ~PointLight() noexcept override;
#pragma endregion

#pragma region //Methods
				DONTDISCARD bool ComputeLighting(const glm::vec3& inpoint, const glm::vec3& innormal,
					const std::vector<std::shared_ptr<Composition::Object>>& objlist,
					const std::shared_ptr<Composition::Object>& obj,
					glm::vec3& color, float& intensity) noexcept override;
#pragma endregion
			};
		}
	}
}

#endif