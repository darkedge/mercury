#version 450 core

layout( location = 0 ) out vec4 FragColor;

in vec2 texcoord;
in vec3 normal;

void main( void ) {
	float tx = 8.0 * texcoord.x;
	float ty = 8.0 * texcoord.y;

	float ipx, ipy;
	modf(tx, ipx);
	modf(ty, ipy);
	int ix = int(ipx);
	int iy = int(ipy);

	if ((ix % 2) == (iy % 2)) {
		FragColor = vec4(1,0,1,1);
	} else {
		FragColor = vec4(0,0,0,1);
	}
}
