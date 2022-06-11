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
#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "../vendor/glm/glm/gtx/matrix_decompose.hpp"
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#ifdef SIE_INCLUDE_GL

#ifndef SIE_GL_INCLUDE_PATH
#define SIE_GL_INCLUDE_PATH "../vendor/glew/include/GL/glew.h"
#endif 

#include SIE_GL_INCLUDE_PATH
#endif

#include "../vendor/glm/glm/gtc/type_ptr.hpp"
#include "general/structures.h"
#include "general/color.h"
#include "general/math.h"
#include "general/helpers.h"
#define SDL_MAIN_HANDLED
#include "../vendor/sdl_repo/include/SDL.h"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "assimp/Importer.hpp"








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