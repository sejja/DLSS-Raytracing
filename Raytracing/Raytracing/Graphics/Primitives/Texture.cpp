//
//	Texture.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 25/06/24
//	Copyright © 2024. All Rights reserved
//

#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Graphics {
	namespace Primitives {
		Texture::Texture() {
			mTramsform = glm::dmat4(1.0);
		}

		Texture::~Texture() {
		}

		glm::dvec3 Texture::GetColor(const glm::dvec2& uvCoords) {
			return glm::dvec3(0.0);
		}

		void Texture::SetTransform(const glm::dvec3& translation, const double& rotation, const glm::dvec3& scale) {
			mTramsform = glm::translate(mTramsform, translation);
			mTramsform = glm::rotate(mTramsform, rotation, glm::dvec3(0.0, 0.0, 1.0));
			mTramsform = glm::scale(mTramsform, scale);
		}

		glm::dvec3 Texture::BlendColors(const std::vector<glm::dvec3>& inputColorList) {
			return glm::dvec3(0.0);
		}

		glm::dvec3 Texture::ApplyTransform(const glm::dvec3& inputVector) {
			return mTramsform * glm::dvec4(inputVector, 1.f);
		}	
	}
}