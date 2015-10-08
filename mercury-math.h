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
	float cell[4];
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

struct mat4 {
	float4 cell[4];
	float4& operator[](int32_t i) {
		return cell[i];
	}
};

static const float3 kXAxis = { 1.0f, 0.0f, 0.0f };
static const float3 kYAxis = { 0.0f, 1.0f, 0.0f };
static const float3 kZAxis = { 0.0f, 0.0f, 1.0f };
static const float3 kNegXAxis = { -1.0f, 0.0f, 0.0f };
static const float3 kNegYAxis = { 0.0f, -1.0f, 0.0f };
static const float3 kNegZAxis = { 0.0f, 0.0f, -1.0f };
static const float2 kZero2f = { 0.0f, 0.0f };
static const float3 kZero3f = { 0.0f, 0.0f, 0.0f };
static const float4 kZero4f = { 0.0f, 0.0f, 0.0f, 0.0f };
static const float kDeg2Rad = 0.01745329251994329576923690768489f;
static const float kRad2Deg = 57.295779513082320876798154814105f;
static const float kPi = 3.14159265358979323846264338327950288f;
//static const float kTau = 2.0f * kPi;
