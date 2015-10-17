#pragma once
#include <math.h>
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
operator*(float2 lhs, float2 rhs) {
	return float2 {
		lhs.x * rhs.x,
		lhs.y * rhs.y,
	};
}

inline float2
operator*(float2 lhs, float rhs) {
	return float2 {
		lhs.x * rhs,
		lhs.y * rhs,
	};
}

inline float2
operator*(float lhs, float2 rhs) {
	return float2 {
		lhs * rhs.x,
		lhs * rhs.y,
	};
}

inline bool
operator==(float2 lhs, float2 rhs) {
	return
		lhs.x == rhs.x && 
		lhs.y == rhs.y;
}

inline bool
operator!=(float2 lhs, float2 rhs) {
	return
		lhs.x != rhs.x || 
		lhs.y != rhs.y;
}

inline float2
operator-(float2 v) {
	return float2 {
		-v.x,
		-v.y,
	};
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
operator*(float3 lhs, float3 rhs) {
	return float3 {
		lhs.x * rhs.x,
		lhs.y * rhs.y,
		lhs.z * rhs.z,
	};
}

inline float3
operator*(float3 lhs, float rhs) {
	return float3 {
		lhs.x * rhs,
		lhs.y * rhs,
		lhs.z * rhs,
	};
}

inline float3
operator*(float lhs, float3 rhs) {
	return float3 {
		lhs * rhs.x,
		lhs * rhs.y,
		lhs * rhs.z,
	};
}

inline bool
operator==(float3 lhs, float3 rhs) {
	return
		lhs.x == rhs.x && 
		lhs.y == rhs.y && 
		lhs.z == rhs.z;
}

inline bool
operator!=(float3 lhs, float3 rhs) {
	return
		lhs.x != rhs.x || 
		lhs.y != rhs.y || 
		lhs.z != rhs.z;
}

inline float3
operator-(float3 v) {
	return float3 {
		-v.x,
		-v.y,
		-v.z,
	};
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
operator*(float4 lhs, float4 rhs) {
	return float4 {
		lhs.x * rhs.x,
		lhs.y * rhs.y,
		lhs.z * rhs.z,
		lhs.w * rhs.w,
	};
}

inline float4
operator*(float4 lhs, float rhs) {
	return float4 {
		lhs.x * rhs,
		lhs.y * rhs,
		lhs.z * rhs,
		lhs.w * rhs,
	};
}

inline float4
operator*(float lhs, float4 rhs) {
	return float4 {
		lhs * rhs.x,
		lhs * rhs.y,
		lhs * rhs.z,
		lhs * rhs.w,
	};
}

inline bool
operator==(float4 lhs, float4 rhs) {
	return
		lhs.x == rhs.x && 
		lhs.y == rhs.y && 
		lhs.z == rhs.z && 
		lhs.w == rhs.w;
}

inline bool
operator!=(float4 lhs, float4 rhs) {
	return
		lhs.x != rhs.x || 
		lhs.y != rhs.y || 
		lhs.z != rhs.z || 
		lhs.w != rhs.w;
}

inline float4
operator-(float4 v) {
	return float4 {
		-v.x,
		-v.y,
		-v.z,
		-v.w,
	};
}

union mat4 {
	float4 v[4];
	float e[16];
};

mat4 Mul(mat4 a, mat4 b);
float4 Mul(mat4 a, float4 b);
mat4 Mul(mat4 a, float b);
float3 Cross(float3 a, float3 b);
float3 QMul(float4 a, float3 b);

extern const float3 kXAxis;
extern const float3 kYAxis;
extern const float3 kZAxis;
extern const float3 kNegXAxis;
extern const float3 kNegYAxis;
extern const float3 kNegZAxis;
extern const float2 kZero2f;
extern const float3 kZero3f;
extern const float4 kZero4f;
extern const float kDeg2Rad;
extern const float kRad2Deg;
extern const float kPi;

mat4 Perspective(float fovy, float aspect, float zNear, float zFar);
mat4 Translation(const float3 &translation);
float3 Forward(float3 position, float4 rotation);
float3 Right(float3 position, float4 rotation);
float3 Up(float3 position, float4 rotation);
float4 Euler(float3 eulerAngle);
float3 Normalize(float3 v);
float3 Cos(float3 v);
float3 Sin(float3 v);
mat4 Transpose(mat4 m);
mat4 MatrixFromQuat(float4 q);
mat4 Inverse(mat4 m);
