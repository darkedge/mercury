#version 450 core

//layout( location = 0 ) out vec4 FragColor;

in vec2 texcoord;
in vec3 normal;
in vec3 worldPos;

layout (location = 0) out vec4 WorldPosOut;
layout (location = 1) out vec4 DiffuseOut;
layout (location = 2) out vec4 NormalOut;
layout (location = 3) out vec4 TexCoordOut;

void main( void ) {
	float tx = 8.0 * texcoord.x;
	float ty = 8.0 * texcoord.y;

	float ipx, ipy;
	modf(tx, ipx);
	modf(ty, ipy);
	int ix = int(ipx);
	int iy = int(ipy);

	if ((ix % 2) == (iy % 2)) {
		DiffuseOut = vec4(1,0,1,1);
	} else {
		DiffuseOut = vec4(0,0,0,1);
	}

	WorldPosOut     = vec4(worldPos, 1.0);					
	//DiffuseOut      = texture(gColorMap, TexCoord0).xyz;	
	NormalOut       = vec4(normalize(normal), 1.0);					
	TexCoordOut     = vec4(texcoord, 0.0, 1.0);	
}
