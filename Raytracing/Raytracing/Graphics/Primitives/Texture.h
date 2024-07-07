//
//	Texture.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 25/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _TEXTURE__H_
#define _TEXTURE__H_

#include <glm/glm.hpp>
#include <vector>

namespace Graphics {
	namespace Primitives {
		class Texture {
		public:
			Texture();
			virtual ~Texture();

			virtual glm::dvec3 GetColor(const glm::dvec2& uvCoords);
			void SetTransform(const glm::dvec3& translation, const double& rotation, const glm::dvec3& scale);
			static glm::dvec3 BlendColors(const std::vector<glm::dvec3>& inputColorList);
			glm::dvec3 ApplyTransform(const glm::dvec3& inputVector);

		private:
			glm::dmat4 mTramsform;

		};
	}
}

#endif