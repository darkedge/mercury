#version 450 core

in vec2 texcoord;
in vec3 normal;
//in vec3 worldPos;

//      |-------------------------------------------|
// RT0: |             depth              | stencil  | 32 bits
//      |-------------------------------------------|
// RT1: |      normal.r       |      normal.g       | 32 bits
//      |-------------------------------------------|
// RT2: | albedo.r | albedo.g | albedo.b | emissive | 32 bits
//      |-------------------------------------------|
// RT3: |  gloss   |  metal   |    -     |    -     | 32 bits
//      |-------------------------------------------|

//layout (location = 0) out vec2 RT0;
layout (location = 0) out vec2 RT1;
layout (location = 1) out vec4 RT2;
layout (location = 2) out vec4 RT3;

void main( void ) {
	float tx = 8.0 * texcoord.x;
	float ty = 8.0 * texcoord.y;

	float ipx, ipy;
	modf(tx, ipx);
	modf(ty, ipy);
	int ix = int(ipx);
	int iy = int(ipy);

	vec4 col = vec4(0,0,0,0);
	if ((ix % 2) == (iy % 2)) {
		col = vec4(1,0,1,1);
	} else {
		col = vec4(0,0,0,1);
	}

#if 0
	WorldPosOut     = vec4(worldPos, 1.0);
	//DiffuseOut      = texture(gColorMap, TexCoord0).xyz;
	NormalOut       = vec4(normalize(normal), 1.0);
#endif
	// RT0 is done by OpenGL?
	//RT0.r = gl_FragCoord.z;
	//RT0.g = ??? // Stencil
	RT1.rg = normal.xy;
	RT2.rgb = col.rgb;
	RT2.a = 255.0; // Emissive
	RT3.r = 16.0 / 255.0; // Gloss
	RT3.g = 0.0 / 255.0; // Metallic
}
