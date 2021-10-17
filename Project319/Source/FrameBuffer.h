#pragma once
#include <cstdint>

#define FRAMEBUFFER_PX_PER_TILE 16
#define FRAMEBUFFER_PX_PER_TILE_POWER 4
#define FRAMEBUFFER_BYTES_PER_TILE 2
#define FRAMEBUFFER_WIDTH uint16_t(512)
#define FRAMEBUFFER_HEIGHT uint16_t(512)

#define FRAMEBUFFER_TILE_W 16
#define FRAMEBUFFER_TILES_X_POWER 5
#define FRAMEBUFFER_TILES_X uint16_t(FRAMEBUFFER_WIDTH/16)
#define FRAMEBUFFER_TILES_Y uint16_t(FRAMEBUFFER_HEIGHT)

#define FRAMEBUFFER_BUF_SIZE uint16_t(FRAMEBUFFER_TILES_X*FRAMEBUFFER_TILES_Y)

enum class BackGroundColor {
    BLACK,
    WHITE
};

struct FrameBuffer {
    uint16_t color[FRAMEBUFFER_BUF_SIZE];

    FrameBuffer();
    FrameBuffer(const FrameBuffer &);
};