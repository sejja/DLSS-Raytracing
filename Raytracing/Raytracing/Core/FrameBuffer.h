//
//	FrameBuffer.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 08/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _FRAMEBUFFER__H_
#define _FRAMEBUFFER__H_

#include <SFML/Graphics.hpp>
#include <memory>
#include "../CommonDefines.h"

namespace Core {
    class FrameBuffer {
    #pragma region //Constructor
    public:
        FrameBuffer(const std::size_t width, const std::size_t height);
    #pragma endregion

    #pragma region //Methods
        void SetSize(const std::size_t width, const std::size_t height);
        void DrawToRenderTarget(sf::RenderTarget& target, sf::RenderStates states);
        void SetColor(const std::size_t x, const std::size_t y, const sf::Color& color) noexcept;
        DONTDISCARD inline std::size_t GetWidth() const noexcept;
        DONTDISCARD inline std::size_t GetHeight() const noexcept;
        DONTDISCARD sf::Color GetColor(const std::size_t x, const std::size_t y) const noexcept;
    private:
        DONTDISCARD inline std::size_t getBufferPixelSize() const;
        DONTDISCARD inline std::size_t pixelIndex(const std::size_t x, const std::size_t y) const;
    #pragma endregion

    #pragma endregion //Members
        std::size_t mWidth, mHeight;
		std::unique_ptr<sf::Uint8[]> mPixels;
		sf::Texture mTexture;
    #pragma endregion
    };

    // ------------------------------------------------------------------------
    /*! Get Buffer Pixel Size
    *
    *   Returns the number of pixels of the Frame Buffer
    */ // ---------------------------------------------------------------------
    std::size_t FrameBuffer::getBufferPixelSize() const {
        return mWidth * mHeight * 4;
    }

    // ------------------------------------------------------------------------
    /*! Pixel Index
    *
	*   Returns the index of a certain pixel given it's coordinates
    */ // ---------------------------------------------------------------------
    std::size_t FrameBuffer::pixelIndex(const std::size_t x, const std::size_t y) const {
        return ((y * mWidth) + x) * 4;
    }

    // ------------------------------------------------------------------------
    /*! Get Width
    *
    *   Returns the Width of the FrameBuffer
    */ // ---------------------------------------------------------------------
    std::size_t FrameBuffer::GetWidth() const noexcept {
        return mWidth;
    }

    // ------------------------------------------------------------------------
    /*! Get Height
    *
    *   Returns the Height of the FrameBuffer
    */ // ---------------------------------------------------------------------
    std::size_t FrameBuffer::GetHeight() const noexcept {
        return mHeight;
    }
}

#endif