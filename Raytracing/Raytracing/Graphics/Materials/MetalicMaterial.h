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
			DONTDISCARD glm::dvec3 ComputeColor(
				const std::vector<std::shared_ptr<Composition::Object>>& objList,
				const std::vector<std::shared_ptr<Primitives::Lighting::Light>>& lightList,
				const std::shared_ptr<Composition::Object>& currObject,
				const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint,
				const Trace::Ray& camRay, glm::dvec2 uv, int reflectioncount) const noexcept override;
			DONTDISCARD glm::dvec3 ComputeSpecular(
				const std::vector<std::shared_ptr<Composition::Object>>& objList,
				const std::vector<std::shared_ptr<Primitives::Lighting::Light>>& lightList,
				const std::shared_ptr<Composition::Object>& currObject,
				const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint,
				const Trace::Ray& camRay) const noexcept;
			void SetColor(const glm::dvec3& color) noexcept;
			void SetShininess(double shininess) noexcept;
			void SetReflectivity(double reflectivity) noexcept;
#pragma endregion

#pragma region //Members
		protected:
			double mReflectivity;
			double mShininess;
			glm::dvec3 mColor;
#pragma endregion
		};
	}
}

#endif