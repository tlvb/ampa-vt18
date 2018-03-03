#version 110
attribute vec2 position;
attribute vec3 color;


varying vec3 c;
const mat4 m = mat4(
	 1.0/640.0,  0.0,       0.0, 0.0,
	 0.0,       -2.0/800.0, 0.0, 0.0,
	 0.0,        0.0,       0.0, 0.0,
	-1.0,        1.0,       0.0, 1.0
);

void main(void) {
	gl_Position = m*vec4(position.x, position.y, 0, 1.0);
	c = color;//vec3(data[2], data[3], data[4]);
}

