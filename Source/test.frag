#version 450

layout(location = 0) in vec3 passFragColor;

layout(location = 0) out vec4 outFragColor;

void main() {
    outFragColor = vec4(passFragColor, 1.0);
}
