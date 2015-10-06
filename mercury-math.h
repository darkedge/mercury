#pragma once

union float2 {
	struct {
		float x, y;
	};
	struct {
		float u, v;
	};
	float cell[2];
};

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

struct mat4 {
	float4 cell[4];
	float4& operator[](int32_t i) {
		return cell[i];
	}
};
