#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoordinates;
out vec2 v_TextureCoordinates;

//uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
    gl_Position = u_Projection * position;
    v_TextureCoordinates = textureCoordinates;
}