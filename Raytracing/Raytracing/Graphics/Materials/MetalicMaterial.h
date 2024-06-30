//
//	MetalicMaterial.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 30/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _METALIC_MATERIAL__H_
#define _METALIC_MATERIAL__H_

#include "../Primitives/Material.h"

namespace Graphics {
	namespace Materials {
		class MetalicMaterial : public Primitives::Material {
#pragma region //Constructors & Destructors
		public:
			MetalicMaterial() noexcept;
			virtual ~MetalicMaterial() noexcept;
#pragma endregion

#pragma region //Methods
			DONTDISCARD glm::vec3 ComputeColor(
				const std::vector<std::shared_ptr<Composition::Object>>& objList,
				const std::vector<std::shared_ptr<Primitives::Lighting::Light>>& lightList,
				const std::shared_ptr<Composition::Object>& currObject,
				const glm::vec3& intersectionPoint, const glm::vec3& normalPoint,
				const Trace::Ray& camRay) const noexcept override;
			DONTDISCARD glm::vec3 ComputeSpecular(
				const std::vector<std::shared_ptr<Composition::Object>>& objList,
				const std::vector<std::shared_ptr<Primitives::Lighting::Light>>& lightList,
				const std::shared_ptr<Composition::Object>& currObject,
				const glm::vec3& intersectionPoint, const glm::vec3& normalPoint,
				const Trace::Ray& camRay) const noexcept;
			void SetColor(const glm::vec3& color) noexcept;
			void SetShininess(float shininess) noexcept;
			void SetReflectivity(float reflectivity) noexcept;
#pragma endregion

#pragma region //Members
		protected:
			float mReflectivity;
			float mShininess;
			glm::vec3 mColor;
#pragma endregion
		};
	}
}

#endif