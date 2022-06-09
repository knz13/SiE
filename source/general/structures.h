#pragma once
#include "../includes.h"
#include "object/game_object.h"

using namespace std;




enum WindowFlag {

    None = 0,
    NotResizeable = 1,
    InitiallyMinimized = 2,
    NotFocused = 4,
    OpenGLDebugContext = 8,
    NotDecorated=16,
    FullScreen=32,
    CoreProfile=64,
    FocusOnShow=128
};

enum ShaderType {
    Vertex=0,
    Fragment,
};


struct KeyEventProperties {
    int key;
    int scancode;
    int action;
    int mods;
};

struct MouseEventProperties {
    glm::vec2 position;
};

struct MouseButtonEventProperties {
    int button;
    int action;
    int mods;
    glm::vec2 position;
};

struct WindowResizedEventProperties {
    int width;
    int height;
};

struct MouseScrollEventProperties {
    glm::vec2 offset;
};










struct ShaderProperties {
    std::string source;
    unsigned int shaderType;


};

struct CameraCreationProperties {
    std::string cameraName = "camera";
    float fov = 60.0f;
    float drawDistance = 100.0f;
    float drawingNearCutoff = 0.1f;
    glm::vec3 initialPos = glm::vec3(0,0,0);
    glm::vec3 initialRotationRadians = glm::vec3(0,0,0);
    glm::vec4 viewPort = glm::vec4(0,0,1,1);
};

struct WindowCreationProperties {
    int width=640;
    int multisamplingCount = 8;
    int height=480;
    
    std::string title="Base Title";
    uint32_t windowFlags = WindowFlag::None;
    int openGLVersionMajor = 4;
    int openGLVersionMinor = 0;
};






struct RayCastHit {
    RayCastHit(entt::entity ent){
        hitObjectID = ent;
    }

    RayCastHit() {};
    
    operator bool() const {
        return ecspp::Registry().valid(hitObjectID);
    };

    entt::entity hitObjectID = entt::null;

};










