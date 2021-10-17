#pragma once

#include <string>
#include <memory>

#include "../Source/Sprite.h"
#include "../Source/FrameBuffer.h"

class ImageManager {
public:
    ImageManager() = delete;

    static std::unique_ptr<Sprite> loadSprite(const std::string &path);
    static void saveSprite(const std::string &path, const Sprite &sprite);

    static std::unique_ptr<FrameBuffer> loadFrameBuffer(const std::string &path);
    static void saveFrameBuffer(const std::string &path, const FrameBuffer &frameBuffer);
};