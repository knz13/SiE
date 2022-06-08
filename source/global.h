#pragma once
#include <iostream>
#include <fstream>
#include <future>
#include <optional>
#include <random>
#include <memory>
#include <unordered_map>
#include <filesystem>
#include <map>
#include <concepts>
#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "../vendor/glm/glm/gtx/matrix_decompose.hpp"
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "../vendor/glew/include/GL/glew.h"
#include "../vendor/glm/glm/gtc/type_ptr.hpp"
#include "general/structures.h"
#include "general/color.h"
#include "general/math.h"
#include "general/helpers.h"
#define SDL_MAIN_HANDLED
#include "../vendor/sdl_repo/include/SDL.h"
using namespace std;






static std::string LoadFileContents(std::string fileName) {
    std::string fileContents = "";

    ifstream stream(fileName);

    std::string line;
    while(getline(stream,line)){
        fileContents += line;
        fileContents += "\n";
    }

    return fileContents;
    


}