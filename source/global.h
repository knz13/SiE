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
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "glm/gtc/type_ptr.hpp"
#include "general/structures.h"
#include "general/color.h"
#include "general/math.h"
#include "general/helpers.h"
#include "SDL.h"
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