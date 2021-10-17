#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Sprite.h"
#include "FrameBuffer.h"
#include "SpriteInstance.h"

struct Scene {
    BackGroundColor bkgColor = BackGroundColor::BLACK;
    std::vector<std::unique_ptr<Sprite>> sprites;
    std::vector<std::string> spriteNames;
    std::vector<SpriteInstance> spriteInstances;
};