#version 410 core

layout (location = 0) in vec4 aPos;

out vec4 vPos;

uniform float xn;
uniform float yn;

uniform float a;
uniform float b;
uniform float c;
uniform float d;

const float pix_size = 1.f;

bool first = true;

float last_x;
float last_y;

int vertex_num = 0;

//x_n+1 = sin(ay_n) + c cos(a x_n);
//y_{n+1} = sin(b x_n) + d cos (b y_n);

void main() {

    if (vertex_num == 0) {
        if (first) {
            last_x = xn;
            last_y = yn;
            first = false;
        } else {
            last_x = sin(a * last_y) + c * cos(a * last_x);
            last_y = sin(b * last_x) + d * cos(b * last_y);
        }
        aPos = vec4(last_x, last_y, -1.0, aPos.w);
        vertex_num++;
    } else if (vertex_num == 1) {
        aPos = vec4(last_x, last_y + pix_size, -1.0, aPos.w);
        vertex_num++;
    } else if (vertex_num == 2) {
        aPos = vec4(last_x + pix_size, last_y + pix_size, -1.0, aPos.w);
        vertex_num++;
    } else if (vertex_num == 3) {
        aPos = vec4(last_x, last_y, -1.0, aPos.w);
        vertex_num++;
    } else if (vertex_num == 4) {

    }
    vPos = aPos;
}