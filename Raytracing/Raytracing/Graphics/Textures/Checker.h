#ifndef _CHECKER__H_
#define _CHECKER__H_

#include "../Primitives/Texture.h"

namespace Graphics {
	namespace Textures {
		class Checker : public Primitives::Texture {
		public:
			Checker();
			virtual ~Checker();

			virtual glm::dvec3 GetColor(const glm::dvec2& uvCoords);
		};
	}
}

#endif