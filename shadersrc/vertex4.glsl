#version 110
uniform mat4 mvp;
attribute vec3 position;
void main(void) {
	gl_Position = mvp * vec4(position.x, position.y, position.z, 1);
}

