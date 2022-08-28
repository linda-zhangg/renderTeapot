#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
//new colour coords for colouring the teapot
uniform float R,G,B;
//new eye position for lighting controls
uniform float x,y,z;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
} f_in;

// framebuffer output
out vec4 fb_color;

void main() {
	// calculate shading - updated with RGB values
	vec3 surfaceColor = vec3(R, G, B);
    vec3 eyePosition = vec3(x,y,z);
	vec3 eye = normalize(eyePosition); // direction towards the eye
	float light = abs(dot(normalize(f_in.normal), eye)); // difference between the surface normal and direction towards the eye
	vec3 finalColor = mix(surfaceColor / 4, surfaceColor, light);

	// output to the frambuffer
	fb_color = vec4(finalColor, 1);
}
