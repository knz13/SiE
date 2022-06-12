#pragma once
#include <iostream>
#include <map>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <memory>
#include <map>
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "general/color.h"
#include "glm/gtc/type_ptr.hpp"
#include "ecspp.h"
#include "yael.h"
#include "../vendor/glew/include/GL/glew.h"



#define GL_SIZEOF(x) TestSize(x)
#define KV_CLASS private: \
                    friend class Registry; friend class Window; friend class Camera;
#define KV_DRAWING_MODE private: friend class Mesh;


static unsigned int TestSize(unsigned int dataType) {
    switch (dataType) {
    case GL_FLOAT:
        return sizeof(float);
    case GL_UNSIGNED_INT:
        return sizeof(unsigned int);
    case GL_UNSIGNED_BYTE:
        return sizeof(unsigned char);
    case GL_BYTE:
        return sizeof(char);
    case GL_INT:
        return sizeof(int);
    default:
        return 0;
    }
}


#define LOG(x) cout << "LOG: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl


#ifdef NDEBUG
#define DEBUG_LOG(x)
#define DEBUG_WARN(x)
#define DEBUG_ERROR(x)
#else
#define DEBUG_LOG(x) cout << "LOG: " << x << endl <<   " At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl
#define DEBUG_WARN(x) cout << "WARNING: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl
#define DEBUG_ERROR(x) cout << "ERROR! -> " << x  << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl;
#endif

static void ClearGLErrors() {
    while (glGetError() != GL_NO_ERROR) {
        ;
    }
}

static bool GetGLError(int line, std::string file) {
    GLenum code = glGetError();
    if (code != GL_NO_ERROR) {
        cout << "OpenGL error '" << std::to_string(code) << "' \nAt line: " << line << " \nIn file: " << file << endl;
        return true;
    }
    return false;
}

#ifdef NDEBUG
#define SIE_GL_CALL(x) x 
#define SIE_GL_CALL_WITH_RESULT(var,x) var = x 
#else
#define SIE_GL_CALL(x) ClearGLErrors(); x; if(GetGLError(__LINE__,__FILE__)) {;}
#define SIE_GL_CALL_WITH_RESULT(var,x) ClearGLErrors(); var = x; if(GetGLError(__LINE__,__FILE__)) {;}
#endif
