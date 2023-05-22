#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "../Lib/stb/stb_image.h"

#include <windows.h>
#include <array>
#include "Texture.h"

struct PNGTexture {
	template <std::size_t N>
	PNGTexture(const std::array<char, N>& png) noexcept : pngData{ png.data() }, pngDataSize{ png.size() } {}

	ImTextureID getTexture() const noexcept {

		if (!texture.get()) {
			int width, height;
			stbi_set_flip_vertically_on_load_thread(false);

			if (const auto data = stbi_load_from_memory((const stbi_uc*)pngData, pngDataSize, &width, &height, nullptr, STBI_rgb_alpha)) {
				texture.init(width, height, data);
				stbi_image_free(data);
			}
			else {
				assert(false);
			}
		}

		return texture.get();
	}

	void clearTexture() const noexcept { texture.clear(); }

private:
	const char* pngData;
	std::size_t pngDataSize;

	mutable Texture texture;
};