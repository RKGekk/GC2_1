#pragma once

#include <string>
#include <memory>

struct Scene;

class SceneLoader {
public:
    static std::unique_ptr<Scene> loadScene(const std::string &path);
    static void saveScene(const std::string &path, const Scene &scene);
};