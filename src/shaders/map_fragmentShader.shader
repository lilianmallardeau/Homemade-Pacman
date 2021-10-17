#version 450 core

in vec4 v_Position;
uniform vec4 u_Color;
uniform bool u_DrawPellet;
out vec4 color;

uniform float pelletSize = .2f;

void main() {
    if (u_DrawPellet) {
        vec2 pos = mod(v_Position.xy, 1.f);
        if (pos.x > .5f) pos.x -= 1.f;
        if (pos.y > .5f) pos.y -= 1.f;
        color = u_Color * smoothstep(pelletSize + 0.05f, pelletSize - 0.05f, length(pos));
    } else {
        color = u_Color;
    }
}