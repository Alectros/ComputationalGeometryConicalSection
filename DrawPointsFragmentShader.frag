#version 330

uniform vec3 color;

void main(void)
{
    gl_FragColor = vec4(color, 1);
}
