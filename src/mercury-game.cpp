#include "mercury-game.h"
#include "mercury-program.h"
#include "mercury-math.h"
#include "glad/glad.h"
#include <vector>

#define KL_V(t) (1 - (t))

static GLuint m_vertexArray = 0;
static GLuint m_vertexBuffer = 0;
static GLuint m_colorBuffer = 0;
static GLuint m_texCoordBuffer = 0;
static GLuint m_indexBuffer = 0;
static size_t m_numTris = 0;
static mat4 m_projection;

static float3 s_playerPosition;
static float4 s_playerRotation;

struct GBuffer {
	GLuint fbo = 0;
	GLuint renderTarget0 = 0;
	GLuint renderTarget1 = 0;
	GLuint renderTarget2 = 0;
	GLuint renderTarget3 = 0;
	GLuint depthTexture = 0;
} s_gbuffer;

void InitGBuffer() {
	 // Create the FBO
	glGenFramebuffers(1, &s_gbuffer.fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s_gbuffer.fbo);

	// Create the gbuffer textures

	// Position
	glGenTextures(1, &s_gbuffer.renderTarget0);
	glBindTexture(GL_TEXTURE_2D, s_gbuffer.renderTarget0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, GetWindowWidth(), GetWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s_gbuffer.renderTarget0, 0);

	// Diffuse
	glGenTextures(1, &s_gbuffer.renderTarget1);
	glBindTexture(GL_TEXTURE_2D, s_gbuffer.renderTarget1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, GetWindowWidth(), GetWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, s_gbuffer.renderTarget1, 0);

	// Normal
	glGenTextures(1, &s_gbuffer.renderTarget2);
	glBindTexture(GL_TEXTURE_2D, s_gbuffer.renderTarget2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, GetWindowWidth(), GetWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, s_gbuffer.renderTarget2, 0);

	// Texcoord
	glGenTextures(1, &s_gbuffer.renderTarget3);
	glBindTexture(GL_TEXTURE_2D, s_gbuffer.renderTarget3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, GetWindowWidth(), GetWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, s_gbuffer.renderTarget3, 0);

	// depth
	glGenTextures(1, &s_gbuffer.depthTexture);
	glBindTexture(GL_TEXTURE_2D, s_gbuffer.depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, GetWindowWidth(), GetWindowHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT,
				  NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, s_gbuffer.depthTexture, 0);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, DrawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void InitSphere() {
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
	GL_TRY(glGenVertexArrays(1, &m_vertexArray));
	GL_TRY(glBindVertexArray(m_vertexArray));

	// Position buffer
	GL_TRY(glGenBuffers(1, &m_vertexBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer));
	if (m_positions.size() > 0) GL_TRY(glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(float3), m_positions.data(), GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(0));

	// Normal buffer
	GL_TRY(glGenBuffers(1, &m_colorBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer));
	if (m_normals.size() > 0) GL_TRY(glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(float3), m_normals.data(), GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(1));

	// Texcoord buffer
	GL_TRY(glGenBuffers(1, &m_texCoordBuffer));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer));
	if (m_texcoords.size() > 0) GL_TRY(glBufferData(GL_ARRAY_BUFFER, m_texcoords.size() * sizeof(float2), m_texcoords.data(), GL_STATIC_DRAW));
	GL_TRY(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr));
	GL_TRY(glEnableVertexAttribArray(2));

	// Index buffer
	GL_TRY(glGenBuffers(1, &m_indexBuffer));
	GL_TRY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer));
	if (m_indices.size() > 0) GL_TRY(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(int32_t), m_indices.data(), GL_STATIC_DRAW));

	m_numTris = m_indices.size() / 3;

	// Unbind
	GL_TRY(glBindVertexArray(0));
	GL_TRY(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_TRY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Init() {
	LoadPrograms();

	InitSphere();

	m_projection = Perspective(60.0f * kDeg2Rad, 16.0f / 9.0f, 0.3f, 1000.0f);

	InitGBuffer();
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

	// bind camera uniform

	// bind program
	BindGeometryProgram();

	// set mvp matrix
	mat4 model = Translation(float3{0.0f, 0.0f, -3.0f});

	mat4 translate = Translation(-s_playerPosition);
	mat4 rotate = Transpose(MatrixFromQuat(s_playerRotation));
	mat4 view = Mul(rotate, translate);
	//mat4 mvp = m_projection * view * model;
	mat4 mvp = Mul(Mul(m_projection, view), model);
	SetGeometryProgramConstants(mvp, model);



	// Geometry pass

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s_gbuffer.fbo);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);







	// render sphere
	GL_TRY(glBindVertexArray(m_vertexArray));
	GL_TRY(glDrawElements(GL_TRIANGLES, (GLsizei)(m_numTris * 3), GL_UNSIGNED_INT, 0));
	GL_TRY(glBindVertexArray(0));










	// Light pass
#if 1
	// Clear default FBO (screen)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind G-buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, s_gbuffer.fbo);
	GLsizei HalfWidth = (GLsizei)(GetWindowWidth() / 2.0f);
	GLsizei HalfHeight = (GLsizei)(GetWindowHeight() / 2.0f);

	// top-left - position
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, GetWindowWidth(), GetWindowHeight(),
					0, HalfHeight, HalfWidth, GetWindowHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);

	// top-right - diffuse
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0, 0, GetWindowWidth(), GetWindowHeight(),
					HalfWidth, HalfHeight, GetWindowWidth(), GetWindowHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);

	// bottom-left - normal
	glReadBuffer(GL_COLOR_ATTACHMENT2);
	glBlitFramebuffer(0, 0, GetWindowWidth(), GetWindowHeight(),
		0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	// bottom-right - texcoord
	glReadBuffer(GL_COLOR_ATTACHMENT3);
	glBlitFramebuffer(0, 0, GetWindowWidth(), GetWindowHeight(),
					HalfWidth, 0, GetWindowWidth(), HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR); 
#endif
}

void Destroy() {
	// TODO
}
