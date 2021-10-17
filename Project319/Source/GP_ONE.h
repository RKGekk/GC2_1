#pragma once

#include <vector>
#include <memory>

#include "Sprite.h"
#include "SpriteInstance.h"
#include "FrameBuffer.h"

#define MAX_SPRITE_COUNT 32

class GP_ONE {
public:
    static Sprite spriteMemory[MAX_SPRITE_COUNT];
    static uint16_t frameBuffer[FRAMEBUFFER_BUF_SIZE];

    GP_ONE() = delete;

    static void loadSprites(const std::vector<std::unique_ptr<Sprite>>& sprites);
    static void clearFrameBuffer(BackGroundColor bkgColor);
    static void drawSpriteInstances(const SpriteInstance *instances, uint16_t instanceCount);
    static void saveFrameBuffer(FrameBuffer &outFrameBuffer);
};
