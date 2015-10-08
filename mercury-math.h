#pragma once
// TODO: Can we do Right-handed Z-up?

union float2 {
	struct {
		float x, y;
	};
	struct {
		float u, v;
	};
	float cell[2];
};

inline float2 &
operator+=(float2 &lhs, float2 rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

inline float2 &
operator-=(float2 &lhs, float2 rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

inline float2 &
operator*=(float2 &lhs, float rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

inline float2
operator+(float2 lhs, float2 rhs) {
	float2 result = lhs;
	result += rhs;
	return result;
}

inline float2
operator-(float2 lhs, float2 rhs) {
	float2 result = lhs;
	result -= rhs;
	return result;
}

inline float2
operator*(float2 lhs, float rhs) {
	float2 result = lhs;
	result *= rhs;
	return result;
}

union float3 {
	struct {
		float x, y, z;
	};
	struct {
		float u, v, w;
	};
	struct {
		float r, g, b;
	};
	struct {
		float2 xy;
		float Ignored0_;
	};
	struct {
		float Ignored1_;
		float2 yz;
	};
	struct {
		float2 uv;
		float Ignored2_;
	};
	struct {
		float Ignored3_;
		float2 vw;
	};
	float cell[3];
};

inline float3 &
operator+=(float3 &lhs, float3 rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

inline float3 &
operator-=(float3 &lhs, float3 rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

inline float3 &
operator*=(float3 &lhs, float rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	return lhs;
}

inline float3
operator+(float3 lhs, float3 rhs) {
	float3 result = lhs;
	result += rhs;
	return result;
}

inline float3
operator-(float3 lhs, float3 rhs) {
	float3 result = lhs;
	result -= rhs;
	return result;
}

inline float3
operator*(float3 lhs, float rhs) {
	float3 result = lhs;
	result *= rhs;
	return result;
}

union float4 {
	struct {
		union {
			float3 xyz;
			struct {
				float x, y, z;
			};
		};

		float w;
	};
	struct {
		union {
			float3 rgb;
			struct {
				float r, g, b;
			};
		};

		float a;
	};
	struct {
		float2 xy;
		float Ignored0_;
		float Ignored1_;
	};
	struct {
		float Ignored2_;
		float2 yz;
		float Ignored3_;
	};
	struct {
		float Ignored4_;
		float Ignored5_;
		float2 zw;
	};
	float e[4];
};

inline float4 &
operator+=(float4 &lhs, float4 rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	lhs.w += rhs.w;
	return lhs;
}

inline float4 &
operator-=(float4 &lhs, float4 rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	lhs.w -= rhs.w;
	return lhs;
}

inline float4 &
operator*=(float4 &lhs, float rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	lhs.w *= rhs;
	return lhs;
}

inline float4
operator+(float4 lhs, float4 rhs) {
	float4 result = lhs;
	result += rhs;
	return result;
}

inline float4
operator-(float4 lhs, float4 rhs) {
	float4 result = lhs;
	result -= rhs;
	return result;
}

inline float4
operator*(float4 lhs, float rhs) {
	float4 result = lhs;
	result *= rhs;
	return result;
}

union mat4 {
	float4 v[4];
	float e[16];
};

mat4 Mul(mat4 a, mat4 b) {
	mat4 result;
	result.e[0] = a.e[0] * b.e[0] + a.e[4] * b.e[1] + a.e[8]  * b.e[2] + a.e[12] * b.e[3];
	result.e[1] = a.e[1] * b.e[0] + a.e[5] * b.e[1] + a.e[9]  * b.e[2] + a.e[13] * b.e[3];
	result.e[2] = a.e[2] * b.e[0] + a.e[6] * b.e[1] + a.e[10] * b.e[2] + a.e[14] * b.e[3];
	result.e[3] = a.e[3] * b.e[0] + a.e[7] * b.e[1] + a.e[11] * b.e[2] + a.e[15] * b.e[3];

	result.e[4] = a.e[0] * b.e[4] + a.e[4] * b.e[5] + a.e[8]  * b.e[6] + a.e[12] * b.e[7];
	result.e[5] = a.e[1] * b.e[4] + a.e[5] * b.e[5] + a.e[9]  * b.e[6] + a.e[13] * b.e[7];
	result.e[6] = a.e[2] * b.e[4] + a.e[6] * b.e[5] + a.e[10] * b.e[6] + a.e[14] * b.e[7];
	result.e[7] = a.e[3] * b.e[4] + a.e[7] * b.e[5] + a.e[11] * b.e[6] + a.e[15] * b.e[7];

	result.e[8]  = a.e[0] * b.e[8] + a.e[4] * b.e[9] + a.e[8]  * b.e[10] + a.e[12] * b.e[11];
	result.e[9]  = a.e[1] * b.e[8] + a.e[5] * b.e[9] + a.e[9]  * b.e[10] + a.e[13] * b.e[11];
	result.e[10] = a.e[2] * b.e[8] + a.e[6] * b.e[9] + a.e[10] * b.e[10] + a.e[14] * b.e[11];
	result.e[11] = a.e[3] * b.e[8] + a.e[7] * b.e[9] + a.e[11] * b.e[10] + a.e[15] * b.e[11];

	result.e[12] = a.e[0] * b.e[12] + a.e[4] * b.e[13] + a.e[8]  * b.e[14] + a.e[12] * b.e[15];
	result.e[13] = a.e[1] * b.e[12] + a.e[5] * b.e[13] + a.e[9]  * b.e[14] + a.e[13] * b.e[15];
	result.e[14] = a.e[2] * b.e[12] + a.e[6] * b.e[13] + a.e[10] * b.e[14] + a.e[14] * b.e[15];
	result.e[15] = a.e[3] * b.e[12] + a.e[7] * b.e[13] + a.e[11] * b.e[14] + a.e[15] * b.e[15];

	return result;
}

float4 Mul(mat4 a, float4 b) {
	float4 result;
	
	result.x = a.e[0] * b.x + a.e[4] * b.y + a.e[8]  * b.z + a.e[12] * b.w;
	result.y = a.e[1] * b.x + a.e[5] * b.y + a.e[9]  * b.z + a.e[13] * b.w;
	result.z = a.e[2] * b.x + a.e[6] * b.y + a.e[10] * b.z + a.e[14] * b.w;
	result.w = a.e[3] * b.x + a.e[7] * b.y + a.e[11] * b.z + a.e[15] * b.w;

	return result;
}

static const float3 kXAxis { 1.0f, 0.0f, 0.0f };
static const float3 kYAxis { 0.0f, 1.0f, 0.0f };
static const float3 kZAxis { 0.0f, 0.0f, 1.0f };
static const float3 kNegXAxis { -1.0f, 0.0f, 0.0f };
static const float3 kNegYAxis { 0.0f, -1.0f, 0.0f };
static const float3 kNegZAxis { 0.0f, 0.0f, -1.0f };
static const float2 kZero2f { 0.0f, 0.0f };
static const float3 kZero3f { 0.0f, 0.0f, 0.0f };
static const float4 kZero4f { 0.0f, 0.0f, 0.0f, 0.0f };
static const float kDeg2Rad = 0.01745329251994329576923690768489f;
static const float kRad2Deg = 57.295779513082320876798154814105f;
static const float kPi = 3.14159265358979323846264338327950288f;
//static const float kTau = 2.0f * kPi;

void LoadProjection(mat4 *result, float fovy, float aspect, float zNear, float zFar) {
	float tanHalfFovy = (float) tan(fovy / 2.0f);
	(*result).v[0] = { 1.0f / (aspect * tanHalfFovy), 0.0f, 0.0f, 0.0f };
	(*result).v[1] = { 0.0f, 1.0f / (tanHalfFovy), 0.0f, 0.0f };
	(*result).v[2] = { 0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -1.0f };
	(*result).v[3] = { 0.0f, 0.0f, -(2.0f * zFar * zNear) / (zFar - zNear), 0.0f };
}

void LoadTranslation(mat4 *result, const float3 &translation) {
	(*result).v[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
	(*result).v[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
	(*result).v[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
	(*result).v[3] = { translation, 1.0f };
}
