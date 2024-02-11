#include <cstring>
#include "image/blender.hpp"

void Blender::Blend(const Image& img, const RectU32& dirtyArea, Pixel* outBuff, bool checkerboard) {
	bool firstPass = true && checkerboard;

	for (std::size_t j = 0; j < img.Layers.size(); ++j) {
		for (u32 y = dirtyArea.y; y < dirtyArea.h; y++) {
			for (u32 x = dirtyArea.x; x < dirtyArea.w; x++) {
				i32 idx = (y * img.w) + x;
				Pixel& frontPixel = img.Layers[j].pixels[idx];
				Pixel& backPixel = outBuff[idx];

				if (firstPass) {
					// checkerboard pattern
					#define PATTERN_SCALE 2
					if (((x / PATTERN_SCALE + y / PATTERN_SCALE) % 2) == 0) {
						backPixel = { 0x80, 0x80, 0x80, 255 };
					} else {
						backPixel = { 0xC0, 0xC0, 0xC0, 255 };
					}
				}

				if (frontPixel.a == 255) {
					backPixel = frontPixel;
					continue;
				}

				backPixel.r = CLAMP_NUM_TO_TYPE(
					static_cast<u16>(
						((u16)frontPixel.r * frontPixel.a + (u16)backPixel.r * (255 - frontPixel.a) / 255 * backPixel.a) / 255
					), u8);
				backPixel.g = CLAMP_NUM_TO_TYPE(
					static_cast<u16>(
						((u16)frontPixel.g * frontPixel.a + (u16)backPixel.g * (255 - frontPixel.a) / 255 * backPixel.a) / 255
					), u8);
				backPixel.b = CLAMP_NUM_TO_TYPE(
					static_cast<u16>(
						((u16)frontPixel.b * frontPixel.a + (u16)backPixel.b * (255 - frontPixel.a) / 255 * backPixel.a) / 255
					), u8);
				backPixel.a = CLAMP_NUM_TO_TYPE(
					static_cast<u16>(
						frontPixel.a + (u16)backPixel.a * (255 - frontPixel.a) / 255
					), u8);
			}
		}
		firstPass = false;
	}
}

