#ifndef _SPRITE__H_
#define _SPRITE__H_

#include <string>
#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include "../Primitives/Texture.h"

namespace Graphics {
	namespace Textures {
		class Sprite : public Primitives::Texture {
		public:
			Sprite() = default;
			Sprite(const std::string& path);
			void SetTexture(const std::string& path);
			virtual glm::dvec3 GetColor(const glm::dvec2& uv) override;
		private:
			sf::Image mTexture;
		};
	}
}

#endif