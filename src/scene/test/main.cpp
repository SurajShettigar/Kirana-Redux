// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include <loader.hpp>

int main(int argc, char **argv)
{
    using namespace kirana::scene;

    Scene scene{};
    std::string base = "";
    std::string path = base + "assets/gltf/Models/ABeautifulGame/glTF/ABeautifulGame.gltf";
    const auto scene_info = loadScene(path, &scene);
    return 0;
}