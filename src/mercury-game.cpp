#include "mercury-game.h"
#include "mercury-program.h"
#include "mercury-math.h"
#include "glad/glad.h"
#include <vector>

#define KL_V(t) (1 - (t))

static struct {
	GLuint vertexArray = 0;
	GLuint vertexBuffer = 0;
	GLuint colorBuffer = 0;
	GLuint texCoordBuffer = 0;
	GLuint indexBuffer = 0;
	size_t numTris = 0;
} s_sphere;

static struct {
	GLuint vertexArray = 0;
	GLuint vertexBuffer = 0;
	GLuint indexBuffer = 0;
	size_t numTris = 0;
} s_quad;

static mat4 s_projection;

static float3 s_playerPosition;
static float4 s_playerRotation;

static struct {
	GLuint fbo = 0;
	GLuint renderTarget0 = 0;
	GLuint renderTarget1 = 0;
	GLuint renderTarget2 = 0;
	GLuint renderTarget3 = 0;
} s_gbuffer;

void _InitGBuffer() {
	 // Create the FBO
	glGenFramebuffers(1, &s_gbuffer.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, s_gbuffer.fbo);

	// Create the gbuffer textures

	//      |-------------------------------------------|
	// RT0: |             depth              | stencil  | 32 bits
	//      |-------------------------------------------|
	// RT1: |      normal.r       |      normal.g       | 32 bits
	//      |-------------------------------------------|
	// RT2: | albedo.r | albedo.g | albedo.b | emissive | 32 bits
	//      |-------------------------------------------|
	// RT3: |  gloss   |  metal   |    -     |    -     | 32 bits
	//      |-------------------------------------------|

	// RT0
	glGenTextures(1, &s_gbuffer.renderTarget0);
	glBindTexture(GL_TEXTURE_2D, s_gbuffer.renderTarget0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, GetWindowWidth(), GetWindowHeight(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, s_gbuffer.renderTarget0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, s_gbuffer.renderTarget0, 0);

	// RT1
	glGenTextures(1, &s_gbuffer.renderTarget1);
	glBindTexture(GL_TEXTURE_2D, s_gbuffer.renderTarget1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, GetWindowWidth(), GetWindowHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s_gbuffer.renderTarget1, 0);

	// RT2
	glGenTextures(1, &s_gbuffer.renderTarget2);
	glBindTexture(GL_TEXTURE_2D, s_gbuffer.renderTarget2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GetWindowWidth(), GetWindowHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, s_gbuffer.renderTarget2, 0);

	// RT3
	glGenTextures(1, &s_gbuffer.renderTarget3);
	glBindTexture(GL_TEXTURE_2D, s_gbuffer.renderTarget3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GetWindowWidth(), GetWindowHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, s_gbuffer.renderTarget3, 0);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, DrawBuffers);

	// Check if this FBO configuration is supported on the GPU
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return;
	}

	// restore default FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void _InitSphere() {
	// Create sphere
	std::vector<float3> m_positions;
	std::vector<float3> m_normals;
	std::vector<float2> m_texcoords;
	std::vector<int32_t> m_indices;

	const int32_t stacks = 50;
	const int32_t slices = 50;
	const float radius = 1.0f;

	for ( int32_t i = 0; i <= stacks; i++ ) {
		// V texture coordinate.
		float v = i / (float) ( stacks );
		float phi = v * kPi;

		for ( int32_t j = 0; j <= slices; j++ ) {
			// U texture coordinate.
			float u = j / (float) ( slices );
			float theta = u * 2.0f * kPi;

			float x = cos( theta ) * sin( phi );
			float y = cos( phi );
			float z = sin( theta ) * sin( phi );

			m_positions.push_back(float3{ x, y, z } * radius);
			m_normals.push_back(float3{ x, y, z });
			m_texcoords.push_back(float2{ 1.0f - u, KL_V(1.0f - v) });
		}
	}

	for ( int32_t i = 0; i < slices * stacks + slices; ++i ) {
		m_indices.insert( m_indices.end(), {
			i, i + slices + 1, i + slices,
			i + slices + 1, i, i + 1
		} );
	}

	// Create VAO
	GL_TRY(glGenVertexArrays(1, &s_sphere.vertexArray));
	GL_TRY(glBindVertexArray(s_sphere.vertexArray));

	// Position buffer
	GL_TRY(glGenBuffers(1, &s_sphere.vertexBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, s_sphere.vertexBuffer));
	if (m_positions.size() > 0) GL_TRY(glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(float3), m_positions.data(), GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(0));

	// Normal buffer
	GL_TRY(glGenBuffers(1, &s_sphere.colorBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, s_sphere.colorBuffer));
	if (m_normals.size() > 0) GL_TRY(glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(float3), m_normals.data(), GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(1));

	// Texcoord buffer
	GL_TRY(glGenBuffers(1, &s_sphere.texCoordBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, s_sphere.texCoordBuffer));
	if (m_texcoords.size() > 0) GL_TRY(glBufferData(GL_ARRAY_BUFFER, m_texcoords.size() * sizeof(float2), m_texcoords.data(), GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(2));

	// Index buffer
	GL_TRY(glGenBuffers(1, &s_sphere.indexBuffer));
	GL_TRY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_sphere.indexBuffer));
	if (m_indices.size() > 0) GL_TRY(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(int32_t), m_indices.data(), GL_STATIC_DRAW));

	s_sphere.numTris = m_indices.size() / 3;

	// Unbind
	GL_TRY(glBindVertexArray(0));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_TRY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void _InitQuad() {
	// Create VAO
	GL_TRY(glGenVertexArrays(1, &s_quad.vertexArray));
	GL_TRY(glBindVertexArray(s_quad.vertexArray));

	const GLfloat positions[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, 1.0f,
	};

	const GLint indices[] = {
		0, 1, 2, 2, 3, 0,
	};

	// Position buffer
	GL_TRY(glGenBuffers(1, &s_quad.vertexBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, s_quad.vertexBuffer));
	GL_TRY(glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float2), positions, GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(0));

	// Index buffer
	GL_TRY(glGenBuffers(1, &s_quad.indexBuffer));
	GL_TRY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_quad.indexBuffer));
	GL_TRY(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int32_t), indices, GL_STATIC_DRAW));

	s_quad.numTris = 2;

	// Unbind
	GL_TRY(glBindVertexArray(0));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_TRY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Init() {
	LoadPrograms();

	_InitSphere();
	_InitQuad();

	s_projection = Perspective(60.0f * kDeg2Rad, 16.0f / 9.0f, 0.3f, 1000.0f);

	_InitGBuffer();
}

void Tick() {
	// input
	{
		static float2 lastRotation;
		static float2 currentRotation;

		if (Input::GetKeyDown(GLFW_KEY_M))
		{
			Input::SetMouseGrabbed(!Input::IsMouseGrabbed());
		}

		// Reset
		if (Input::GetKeyDown(GLFW_KEY_R))
		{
			currentRotation = lastRotation = { 0, 0 };
			s_playerPosition = { 0, 0, 0 };
			s_playerRotation = { 0, 0, 0, 1 };
		}

		if (Input::IsMouseGrabbed())
		{
			// Rotation
			const float ROT_SPEED = 0.0025f;
			currentRotation -= ROT_SPEED * Input::GetMouseDelta();
			if (currentRotation.y < -90.0f * kDeg2Rad)
			{
				currentRotation.y = -90.0f * kDeg2Rad;
			}
			if (currentRotation.y > 90.0f * kDeg2Rad)
			{
				currentRotation.y = 90.0f * kDeg2Rad;
			}
			if (currentRotation.x != lastRotation.x || currentRotation.y != lastRotation.y)
			{
				s_playerRotation = Euler(float3{ currentRotation.y, currentRotation.x, 0 });
				lastRotation = currentRotation;
			}
		}

		// Translation
		const float SPEED = 5.0f;
		float3 translation { 0, 0, 0 };
		if (Input::GetKey(GLFW_KEY_W))		translation += Forward(s_playerPosition, s_playerRotation);
		if (Input::GetKey(GLFW_KEY_A))		translation -= Right(s_playerPosition, s_playerRotation);
		if (Input::GetKey(GLFW_KEY_S))		translation -= Forward(s_playerPosition, s_playerRotation);
		if (Input::GetKey(GLFW_KEY_D))		translation += Right(s_playerPosition, s_playerRotation);
		if (Input::GetKey(GLFW_KEY_LEFT_CONTROL) || Input::GetKey(GLFW_KEY_C) || Input::GetKey(GLFW_KEY_LEFT_SHIFT)) translation -= float3{ 0, 1, 0 };
		if (Input::GetKey(GLFW_KEY_SPACE)) translation += float3{ 0, 1, 0 };
		if (translation != float3{ 0, 0, 0 })
		{
			float3 pos = s_playerPosition;
			pos += Normalize(translation) * SPEED * GetDeltaTime();
			s_playerPosition = pos;
			//printf("pos: %.1f, %.1f, %.1f\n", s_player->GetPosition().x, s_player->GetPosition().y, s_player->GetPosition().z);
		}
	}

	mat4 view;
	// Geometry pass
	{
		BindGeometryProgram();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s_gbuffer.fbo);
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		
		// render sphere
		{
			mat4 model = Translation(float3{ 0.0f, 0.0f, -3.0f });
			mat4 translate = Translation(-s_playerPosition);
			mat4 rotate = Transpose(MatrixFromQuat(s_playerRotation));
			view = Mul(rotate, translate);
			mat4 mvp = Mul(Mul(s_projection, view), model);
			SetGeometryProgramConstants(mvp, model);
			GL_TRY(glBindVertexArray(s_sphere.vertexArray));
			GL_TRY(glDrawElements(GL_TRIANGLES, (GLsizei)(s_sphere.numTris * 3), GL_UNSIGNED_INT, 0));
			GL_TRY(glBindVertexArray(0));
		}

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
	}


	// Begin light pass
	{
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	// IBL pass
	{
		BindIBLProgram();
		SetIBLProgramConstants(s_gbuffer.renderTarget0, s_gbuffer.renderTarget1, s_gbuffer.renderTarget2, s_gbuffer.renderTarget3, Inverse(view), Inverse(s_projection));

		// Render screen quad
		GL_TRY(glBindVertexArray(s_quad.vertexArray));
		GL_TRY(glDrawElements(GL_TRIANGLES, (GLsizei)(s_quad.numTris * 3), GL_UNSIGNED_INT, 0));
		GL_TRY(glBindVertexArray(0));
	}
}

void Destroy() {
	// TODO
}
