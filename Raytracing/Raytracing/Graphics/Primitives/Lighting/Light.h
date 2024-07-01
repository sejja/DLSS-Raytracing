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
				DONTDISCARD virtual inline bool ComputeLighting( const glm::dvec3& inpoint, const glm::dvec3& innormal,
					const std::vector<std::shared_ptr<Composition::Object>>& objlist,
					const std::shared_ptr<Composition::Object>& obj,
					glm::dvec3& color, double& intensity);
				void inline SetColor(const glm::dvec3& color);
				void inline SetPosition(const glm::dvec3& position);
				DONTDISCARD inline glm::dvec3 GetPosition() const noexcept;
				DONTDISCARD inline glm::dvec3 GetColor() const noexcept;
#pragma endregion

#pragma region //Members
			protected:
				glm::dvec3 mColor;
				glm::dvec3 mPosition;
				double mIntensity;
#pragma endregion
			};

			// ------------------------------------------------------------------------
			/*! ComputeLighting
			*
			*   Computes the lighting for the given ray
			*/ // ---------------------------------------------------------------------
			bool Light::ComputeLighting(const glm::dvec3& inpoint, const glm::dvec3& innormal,
				const std::vector<std::shared_ptr<Composition::Object>>& objlist,
				const std::shared_ptr<Composition::Object>& obj,
				glm::dvec3& color, double& intensity) {
				return false;
			}

			// ------------------------------------------------------------------------
			/*! Set Color
			*
			*   Sets the Color of a Light source
			*/ // ---------------------------------------------------------------------
			void Light::SetColor(const glm::dvec3& color) {
				mColor = color;
			}

			// ------------------------------------------------------------------------
			/*! Set Position
			*
			*   Sets the Position of the Light source
			*/ // ---------------------------------------------------------------------
			void Light::SetPosition(const glm::dvec3& position) {
				mPosition = position;
			}
			
			// ------------------------------------------------------------------------
			/*! Get Position
			*
			*   Returns the Position of the light
			*/ // ---------------------------------------------------------------------
			glm::dvec3 Light::GetPosition() const noexcept {
				return mPosition;
			}

			// ------------------------------------------------------------------------
			/*! Get Color
			*
			*   Returns the Color of the light
			*/ // ---------------------------------------------------------------------
			glm::dvec3 Light::GetColor() const noexcept {
				return mColor;
			}
		}
	}
}

#endif