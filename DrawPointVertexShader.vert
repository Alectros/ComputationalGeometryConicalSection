#version 330
attribute highp vec2 position;

uniform mat3 matrix;

void main(void)
{
    gl_Position = vec4(matrix * vec3(position, 1), 1);
}
