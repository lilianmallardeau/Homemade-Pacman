#version 450 core

layout(location = 0) in vec4 position;

//uniform mat4 u_View;
uniform mat4 u_Projection;
out vec4 v_Position;

void main() {
    gl_Position = u_Projection * position;
    v_Position = position;
}