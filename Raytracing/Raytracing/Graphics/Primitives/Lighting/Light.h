//
//	Light.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 24/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _LIGHT__H_
#define _LIGHT__H_

#include <memory>
#include <vector>
#include "../../../Composition/Object.h"
#include "../../../Trace/Ray.h"

namespace Graphics {
	namespace Primitives {
		namespace Lighting {
			class Light {
#pragma region //Constructors & Destructors
			public:
				Light();
				virtual ~Light();
#pragma endregion

#pragma region //Methods
				DONTDISCARD virtual inline bool ComputeLighting( const glm::vec3& inpoint, const glm::vec3& innormal,
					const std::vector<std::shared_ptr<Composition::Object>>& objlist,
					const std::shared_ptr<Composition::Object>& obj,
					glm::vec3& color, float& intensity);
				void inline SetColor(const glm::vec3& color);
				void inline SetPosition(const glm::vec3& position);
				DONTDISCARD inline glm::vec3 GetPosition() const noexcept;
				DONTDISCARD inline glm::vec3 GetColor() const noexcept;
#pragma endregion

#pragma region //Members
			protected:
				glm::vec3 mColor;
				glm::vec3 mPosition;
				float mIntensity;
#pragma endregion
			};

			// ------------------------------------------------------------------------
			/*! ComputeLighting
			*
			*   Computes the lighting for the given ray
			*/ // ---------------------------------------------------------------------
			bool Light::ComputeLighting(const glm::vec3& inpoint, const glm::vec3& innormal,
				const std::vector<std::shared_ptr<Composition::Object>>& objlist,
				const std::shared_ptr<Composition::Object>& obj,
				glm::vec3& color, float& intensity) {
				return false;
			}

			// ------------------------------------------------------------------------
			/*! Set Color
			*
			*   Sets the Color of a Light source
			*/ // ---------------------------------------------------------------------
			void Light::SetColor(const glm::vec3& color) {
				mColor = color;
			}

			// ------------------------------------------------------------------------
			/*! Set Position
			*
			*   Sets the Position of the Light source
			*/ // ---------------------------------------------------------------------
			void Light::SetPosition(const glm::vec3& position) {
				mPosition = position;
			}
			
			// ------------------------------------------------------------------------
			/*! Get Position
			*
			*   Returns the Position of the light
			*/ // ---------------------------------------------------------------------
			glm::vec3 Light::GetPosition() const noexcept {
				return mPosition;
			}

			// ------------------------------------------------------------------------
			/*! Get Color
			*
			*   Returns the Color of the light
			*/ // ---------------------------------------------------------------------
			glm::vec3 Light::GetColor() const noexcept {
				return mColor;
			}
		}
	}
}

#endif