#include "GP_ONE.h"

Sprite GP_ONE::spriteMemory[MAX_SPRITE_COUNT];
uint16_t GP_ONE::frameBuffer[FRAMEBUFFER_BUF_SIZE];

void GP_ONE::loadSprites(const std::vector<std::unique_ptr<Sprite>>& sprites) {
	for (uint16_t i = 0u; i < sprites.size(); ++i) {
		GP_ONE::spriteMemory[i] = *sprites.at(i);
	}
}

void GP_ONE::clearFrameBuffer(BackGroundColor bkgColor) {
	uint16_t c = bkgColor == BackGroundColor::BLACK ? 0u : -1;
	for (uint16_t i = 0u; i < FRAMEBUFFER_BUF_SIZE; ++i) {
		frameBuffer[i] = c;
	}
}

inline uint16_t dvu(uint16_t val, uint16_t power) {
	return (val + (1u << (power - 1u))) >> power;
}

void GP_ONE::drawSpriteInstances(const SpriteInstance *instances, uint16_t instanceCount) {
	for (uint16_t i = 0u; i < instanceCount; ++i) {
		uint16_t x = (instances + i)->x;
		uint16_t y = (instances + i)->y;
		uint16_t ind = (instances + i)->ind;
		bool odd = x & 1u;

		uint16_t start_tile_x = x >> FRAMEBUFFER_PX_PER_TILE_POWER;
		//uint16_t start_tile_x = dvu(x, FRAMEBUFFER_PX_PER_TILE_POWER);
		//uint16_t start_in_tile_x = x & (FRAMEBUFFER_PX_PER_TILE_POWER - 1);
		uint16_t start_in_tile_x = x - start_tile_x * FRAMEBUFFER_TILE_W;
		uint16_t fb_start_index = (y << FRAMEBUFFER_TILES_X_POWER) + start_tile_x;

		if (start_in_tile_x) {
			std::vector<int> ddasf;
		}

		for (uint16_t sp_y = 0u; sp_y < SPRITE_TILES_Y; ++sp_y) {
			for (uint16_t sp_x = 0u; sp_x < SPRITE_TILES_X; ++sp_x) {
				uint16_t sp_index = (sp_y << SPRITE_TILES_X_POWER) + sp_x;
				uint16_t fb_index_offset = fb_start_index + (sp_y << FRAMEBUFFER_TILES_X_POWER) + sp_x;
				uint16_t fbc = frameBuffer[fb_index_offset];
				uint16_t spc = spriteMemory[ind].color[sp_index] >> start_in_tile_x;
				uint16_t a = spriteMemory[ind].alpha[sp_index] >> start_in_tile_x;
				frameBuffer[fb_index_offset] = (spc & a) | (fbc & ~a);
				//frameBuffer[fb_index_offset] = spc;
			}
		}
	}
}

void GP_ONE::saveFrameBuffer(FrameBuffer &outFrameBuffer) {
	for (uint16_t i = 0u; i < FRAMEBUFFER_BUF_SIZE; ++i) {
		outFrameBuffer.color[i] = frameBuffer[i];
	}
}