#include "ImageManager.h"

#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cstdint>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

//static uint16_t encodeTile(const stbi_uc* pixels, uint32_t width, uint32_t channels, uint32_t i, uint32_t tile_j, uint32_t c) {
//    uint16_t tile = 0u;
//
//    for (uint32_t local_j = 0u; local_j < 16u; ++local_j) {
//        uint32_t j = local_j + tile_j * 16u;
//        uint16_t bit = pixels[i * (width * channels) + j * channels + c] / 255u;
//        tile = (tile << 1) | bit;
//    }
//
//    return tile;
//}
//
//static void decodeTile(uint16_t tile, stbi_uc* pixels, uint32_t width, uint32_t channels, uint32_t i, uint32_t tile_j, uint32_t c) {
//    for (uint32_t local_j = 15u; local_j >= 0u; --local_j) {
//        uint32_t j = local_j + tile_j * 16u;
//        pixels[i * (width * channels) + j * channels + c] = (tile & 1u) * 255u;
//        tile = tile >> 1;
//    }
//}

static uint16_t encodeTile(const stbi_uc* pixels, int width, int channels, int i, int tile_j, int c) {
    uint16_t tile = 0;

    for (int local_j = 0; local_j < 16; ++local_j) {
        int j = local_j + tile_j * 16;
        uint16_t bit = pixels[i * (width * channels) + j * channels + c] / 255;
        tile = (tile << 1) | bit;
    }

    return tile;
}

static void decodeTile(uint16_t tile, stbi_uc* pixels, int width, int channels, int i, int tile_j, int c) {
    for (int local_j = 15; local_j >= 0; --local_j) {
        int j = local_j + tile_j * 16;
        pixels[i * (width * channels) + j * channels + c] = (tile & 1) * 255;
        tile = tile >> 1;
    }
}

static void saveImage(const std::string &path, stbi_uc *pixels, uint32_t width, uint32_t height, uint32_t channels) {
    size_t len = path.size();
    if (strcmp(&(path.data()[len - 3u]), "png") == 0u) {
        stbi_write_png(path.data(), width, height, channels, pixels, 0u);
    }
    else if (strcmp(&(path.data()[len - 3u]), "bmp") == 0u) {
        stbi_write_bmp(path.data(), width, height, channels, pixels);
    }
    else if (strcmp(&(path.data()[len - 3u]), "tga") == 0u) {
        stbi_write_tga(path.data(), width, height, channels, pixels);
    }
    else if (strcmp(&(path.data()[len - 3u]), "jpg") == 0u) {
        stbi_write_jpg(path.data(), width, height, channels, pixels, 100u);
    }
    else {
        std::stringstream ss;
        ss << "Image " << path << " has unsupported extension";
        throw std::runtime_error(ss.str());
    }
}

std::unique_ptr<Sprite> ImageManager::loadSprite(const std::string &path) {
    int width;
    int height;
    int channels;
    stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, 2);
    if (!pixels) {
        std::stringstream ss;
        ss << "Failed to load image on path: " << path;
        throw std::runtime_error(ss.str());
    }
    if ((width != SPRITE_TILES_X * 16u) || (height != SPRITE_TILES_Y)) {
        std::stringstream ss;
        ss << "Image " << path << " has incorrect resolution" << width << "x" << height;
        throw std::runtime_error(ss.str());
    }
    if (channels != 2) {
        std::stringstream ss;
        ss << "Image " << path << " has incorrect channel count = " << channels;
        throw std::runtime_error(ss.str());
    }

    std::unique_ptr<Sprite> outSprite = std::make_unique<Sprite>();
    for (uint16_t i = 0u; i < SPRITE_TILES_Y; ++i) {
        for (uint16_t j = 0u; j < SPRITE_TILES_X; ++j) {
            outSprite->color[i * SPRITE_TILES_X + j] = encodeTile(pixels, width, channels, i, j, 0u);
            outSprite->alpha[i * SPRITE_TILES_X + j] = encodeTile(pixels, width, channels, i, j, 1u);
        }
    }

    return outSprite;
}

std::unique_ptr<FrameBuffer> ImageManager::loadFrameBuffer(const std::string &path) {
    int width;
    int height;
    int  channels;
    stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, 1);
    if (!pixels) {
        std::stringstream ss;
        ss << "Failed to load image on path: " << path;
        throw std::runtime_error(ss.str());
    }
    if ((width != FRAMEBUFFER_TILES_X*16) || (height != FRAMEBUFFER_TILES_Y)) {
        std::stringstream ss;
        ss << "Image " << path << " has incorrect resolution" << width << "x" <<height;
        throw std::runtime_error(ss.str());
    }
    if (channels != 1) {
        std::stringstream ss;
        ss << "Image " << path << " has incorrect channel count = " << channels;
        throw std::runtime_error(ss.str());
    }

    std::unique_ptr<FrameBuffer> outFrameBuffer = std::make_unique<FrameBuffer>();
    for (uint16_t i = 0u; i < FRAMEBUFFER_TILES_Y; ++i) {
        for (uint16_t j = 0u; j < FRAMEBUFFER_TILES_X; ++j) {
            outFrameBuffer->color[i * FRAMEBUFFER_TILES_X + j] = encodeTile(pixels, width, channels, i, j, 0u);
        }
    }

    return outFrameBuffer;
}

void ImageManager::saveSprite(const std::string &path, const Sprite &sprite) {
    uint32_t width = SPRITE_TILES_X * 16u;
    uint32_t height = SPRITE_TILES_Y;
    uint32_t channels = 2u;
    auto *pixels = new stbi_uc[width * height * channels];
    for (uint16_t i = 0u; i < SPRITE_TILES_Y; ++i) {
        for (uint16_t j = 0u; j < SPRITE_TILES_X; ++j) {
            decodeTile(sprite.color[i * SPRITE_TILES_X + j], pixels, width, channels, i, j, 0);
            decodeTile(sprite.alpha[i * SPRITE_TILES_X + j], pixels, width, channels, i, j, 1);
        }
    }
    saveImage(path, pixels, width, height, channels);
}

void ImageManager::saveFrameBuffer(const std::string &path, const FrameBuffer &frameBuffer) {
    uint32_t width = FRAMEBUFFER_TILES_X * 16u;
    uint32_t height = FRAMEBUFFER_TILES_Y;
    auto *pixels = new stbi_uc[width * height];
    for (uint16_t i = 0u; i < FRAMEBUFFER_TILES_Y; ++i) {
        for (uint16_t j = 0u; j < FRAMEBUFFER_TILES_X; ++j) {
            decodeTile(frameBuffer.color[i * FRAMEBUFFER_TILES_X + j], pixels, width, 1, i, j, 0u);
        }
    }
    saveImage(path, pixels, width, height, 1u);
}

