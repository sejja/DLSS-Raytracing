#include "Sprite.h"

namespace Graphics {
	namespace Textures {
		Sprite::Sprite(const std::string& path) {
			SetTexture(path);
		}
		void Sprite::SetTexture(const std::string& path) {
			if (!mTexture.loadFromFile(path))
				throw std::runtime_error("Failed to load texture from file: " + path);
		}

		glm::dvec3 Sprite::GetColor(const glm::dvec2& uv) {
			glm::dvec2 inputLoc = uv;
			glm::dvec3 newLoc = ApplyTransform(glm::dvec3(inputLoc, 0));
			double u = newLoc.x;
			double v = newLoc.y;

			int x = static_cast<int>(round(((u + 1.0) / 2.0) * static_cast<double>(mTexture.getSize().x)));
			int y = mTexture.getSize().y - (static_cast<int>(round(((v + 1.0) / 2.0) * static_cast<double>(mTexture.getSize().y))));


			x = ((x % mTexture.getSize().x) + mTexture.getSize().x) % mTexture.getSize().x;
			y = ((y % mTexture.getSize().y) + mTexture.getSize().y) % mTexture.getSize().y;

			if ((x >= 0) && (x < mTexture.getSize().x) && (y >= 0) && (y < mTexture.getSize().y)) {

				auto pxl = mTexture.getPixel(x, y);

				return glm::dvec3(static_cast<double>(pxl.r) / 255.0, static_cast<double>(pxl.g) / 255.0, static_cast<double>(pxl.g) / 255.0);
			}
		}
	}
}