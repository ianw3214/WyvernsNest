#version 330 core

layout(location = 0) in vec4 position;
out vec2 texCoord;

mat4 ortho(float left, float right, float bottom, float top) {
    return mat4(vec4(2.0 / (right - left), 0, 0, 0), vec4(0, 2.0 / top - bottom, 0, 0), vec4(0, 0, -1, 0), vec4(-(right + left) / (right - left), -(top + bottom) / (top - bottom), 0, 1));
}

void main() {
    gl_Position = ortho(0, 1280, 0, 720) * vec4(position.xy, 0, 1);
    texCoord = position.zw;
}