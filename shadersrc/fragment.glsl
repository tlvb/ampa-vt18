#version 110
varying vec3 c;
void main(void) {
	gl_FragColor = vec4(c, 1.0);
	//gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
