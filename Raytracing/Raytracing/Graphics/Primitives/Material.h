//
//	Material.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 30/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _MATERIAL__H_
#define _MATERIAL__H_

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "../../Composition/Object.h"
#include "../../Trace/Ray.h"
#include "Lighting/Light.h"
#include "Texture.h"

namespace Graphics {
	namespace Primitives {
		class Material {
#pragma region //Constructors & Destructors
		public:
			Material() noexcept;
			virtual ~Material() noexcept;
#pragma endregion

#pragma region //Methods
			DONTDISCARD virtual glm::dvec3 ComputeColor(
				const std::vector<std::shared_ptr<Composition::Object>>& objList, 
				const std::vector<std::shared_ptr<Lighting::Light>>& lightList,
				const std::shared_ptr<Composition::Object>& currObject,
				const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint,
				const Trace::Ray& camRay) const noexcept;
			DONTDISCARD static glm::dvec3 ComputeColorDiffuse(
				const std::vector<std::shared_ptr<Composition::Object>>& objList,
				const std::vector<std::shared_ptr<Lighting::Light>>& lightList,
				const std::shared_ptr<Composition::Object>& currObject,
				const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint,
				const glm::dvec3& color) noexcept;
			DONTDISCARD virtual glm::dvec3 ComputeColorReflection(
				const std::vector<std::shared_ptr<Composition::Object>>& objList,
				const std::vector<std::shared_ptr<Lighting::Light>>& lightList,
				const std::shared_ptr<Composition::Object>& currObject,
				const glm::dvec3& intersectionPoint, const glm::dvec3& normalPoint,
				const Trace::Ray& camRay) const noexcept;
			bool CastRay(const Trace::Ray& ray, 
								const std::vector<std::shared_ptr<Composition::Object>>& objList,	
								std::shared_ptr<Composition::Object>& closestobj, 
								glm::dvec3& inpoint, glm::dvec3& innormal, glm::dvec3& outcolor) const noexcept;
			void SetTexture(const std::shared_ptr<Texture>& texture) noexcept;
#pragma endregion

#pragma region //Members
			inline static int mReflectionRayCount;
			inline static int mReflectionCountMax;
		protected:
			glm::dvec3 mColor;
			std::shared_ptr<Texture> mTexture;
#pragma endregion
		};
	}
}

#endif