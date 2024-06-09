//
//	FrameBuffer.cpp
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 08/06/24
//	Copyright © 2024. All Rights reserved
//

#include "FrameBuffer.h"

namespace Core {
    // ------------------------------------------------------------------------
    /*! Custom Constructor
    *
	*   Creates an SFML Framebuffer with a given dimension
    */ // ---------------------------------------------------------------------
    FrameBuffer::FrameBuffer(const std::size_t width, const std::size_t height) :
        mWidth{ 0 }, mHeight{ 0 } {
        SetSize(width, height);
    }

    // ------------------------------------------------------------------------
    /*! Get Color
    *
	*   Returns the Color of a certain pixel given it's coordinates
    */ // ---------------------------------------------------------------------
    sf::Color FrameBuffer::GetColor(const std::size_t x, const std::size_t y) const noexcept {
        std::size_t indexi = pixelIndex(x, y);

        return sf::Color(
            mPixels[indexi++],
            mPixels[indexi++],
            mPixels[indexi++],
            mPixels[indexi]
        );
    }

    // ------------------------------------------------------------------------
    /*! Set Color
    *
	*   Sets the Color of a Pixel given it's coordinates and the color
    */ // ---------------------------------------------------------------------
    void FrameBuffer::SetColor(const std::size_t x, const std::size_t y, const sf::Color& color) noexcept {
        std::size_t indexi = pixelIndex(x, y);

        mPixels[indexi++] = color.r;
        mPixels[indexi++] = color.g;
        mPixels[indexi++] = color.b;
        mPixels[indexi] = color.a;
    }

    // ------------------------------------------------------------------------
    /*! Draw to Render Target
    *
	*   Converts the Pixels into a Texture and draws it to a Render Target
    */ // ---------------------------------------------------------------------
    void FrameBuffer::DrawToRenderTarget(sf::RenderTarget& target, sf::RenderStates states) {
        mTexture.update(mPixels.get());
        target.draw(sf::Sprite(mTexture), states);
    }

    // ------------------------------------------------------------------------
    /*! Set Size
    *
    *   Sets the Size of the new FrameBuffer
    */ // ---------------------------------------------------------------------
    void FrameBuffer::SetSize(const std::size_t width, const std::size_t height) {
        sf::Texture newTexture;

        mWidth = width, mHeight = height;
        newTexture.create(static_cast<unsigned>(mWidth), static_cast<unsigned>(mHeight));
        mTexture = newTexture;
        mPixels = std::make_unique<sf::Uint8[]>(getBufferPixelSize());
    }
}