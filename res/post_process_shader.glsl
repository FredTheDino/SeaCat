#ifdef VERT

layout (location=0) in vec2 pos;
layout (location=1) in vec2 uv;

out vec2 pass_uv;
out vec4 pass_color;

void main() {
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    pass_uv = vec2(uv);
}

#else

in vec2 pass_uv;
in vec4 pass_color;

out vec4 color;

void main() {
    if (num_active_samplers == 1) {
        color = texture(screen_samplers[0], pass_uv);
    } else {
        if (pass_uv.x > 0.5) {
            color = texture(screen_samplers[1], pass_uv);
        } else {
            color = texture(screen_samplers[0], pass_uv);
        }
    }
    vec2 coords = (pass_uv - 0.5) * radius;
    vec4 scaler = vec4(1.0 - clamp(strength * length(coords), 0.0, 1.0));
    scaler.a = 1.0;
    color = color * scaler; 
}

#endif
