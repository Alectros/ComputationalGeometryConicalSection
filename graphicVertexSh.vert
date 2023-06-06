#version 330
attribute highp vec2 position;

out vec2 fragPos;

void main(void)
{
    fragPos = position;
    gl_Position = vec4(position, -1, 1);
}
