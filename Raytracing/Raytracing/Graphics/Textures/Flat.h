#ifndef _FLAT__H_
#define _FLAT__H_

#include "../Primitives/Texture.h"

namespace Graphics {
	namespace Textures {
		class Flat : public Primitives::Texture {
		public:
			Flat();
			virtual ~Flat();

			virtual glm::dvec3 GetColor(const glm::dvec2& uvCoords);
		};
	}
}

#endif