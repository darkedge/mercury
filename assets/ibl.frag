#version 450 core

//      |-------------------------------------------|
// RT0: |             depth              | stencil  | 32 bits
//      |-------------------------------------------|
// RT1: |      normal.r       |      normal.g       | 32 bits
//      |-------------------------------------------|
// RT2: | albedo.r | albedo.g | albedo.b | emissive | 32 bits
//      |-------------------------------------------|
// RT3: |  gloss   |  metal   |    -     |    -     | 32 bits
//      |-------------------------------------------|

uniform sampler2D RT0;
uniform sampler2D RT1;
uniform sampler2D RT2;
uniform sampler2D RT3;
uniform samplerCube CubeMap;

uniform mat4 InverseView;
uniform mat4 InverseProj;

in vec2 texcoord;

layout (location = 0) out vec4 ColorOut;

const int NUM_ROUGHNESS_MIPS = 6;

vec3 unpackNormal(vec2 g) {
	float nz = dot(g, g) * 2 - 1;
	return vec3(normalize(g) * sqrt(1 - nz * nz), nz);
}

vec3 reconstructPos(vec2 uv, float d) {
	vec4 p = vec4(vec3(uv * 2.0 - 1.0, d), 1.0);
	p = InverseProj * p;
	return p.xyz / p.w;
}

vec3 getReflectionColor(vec3 rayO, vec3 rayD, float glossiness) {
	float mip = (1.0 - glossiness) * float(NUM_ROUGHNESS_MIPS - 1);
	vec4 wsRayD = InverseView * vec4(rayD, 0.0);
	vec3 col = textureLod(CubeMap, wsRayD.xyz, mip).rgb;
	return col;
}

void main( void ) {
	float depth = texture(RT0, texcoord).r;
	vec3 position = reconstructPos(texcoord, depth);
	vec3 normal = unpackNormal(texture(RT1, texcoord).xy);
	vec4 rt2 = texture(RT2, texcoord);
	vec4 rt3 = texture(RT3, texcoord);
	vec3 albedo = rt2.rgb;
	float glossiness = rt3.r;
	float metalness = rt3.g;

	const float minReflectivity = 0.04;
	float reflectivity = mix(minReflectivity, 1.0, metalness);
	vec3 V = normalize(-position);
	vec3 refl = reflect(-V, normal);
	vec3 reflCol = getReflectionColor(position, refl, glossiness);

	vec3 ambient = textureLod(CubeMap, normal, float(NUM_ROUGHNESS_MIPS - 1)).rgb;
	reflCol *= mix(vec3(1,1,1), albedo, metalness);
	vec3 col = mix(albedo /* * ambient */, reflCol, reflectivity);
	#if 0
	if (depth == 1.0) {
		// If infinitely far away, use albedo directly
		col = rt2.rgb;
	}
	#endif
	col.x = pow(col.x, 1.0 / 2.2);
	col.y = pow(col.y, 1.0 / 2.2);
	col.z = pow(col.z, 1.0 / 2.2);

	ColorOut = vec4(col, 1.0);
}
