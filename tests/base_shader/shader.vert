#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;


uniform vec3 UMyIdentifier;
uniform mat4 MVP;

void main(){
    
    gl_Position = MVP * vec4(pos,1.0);
}