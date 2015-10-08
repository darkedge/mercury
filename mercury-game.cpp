#include "mercury-game.h"
#include "mercury-program.h"
#include <vector>

#define GL_TRY(x) x
#define KL_V(t) (1 - (t))

static GLuint m_vertexArray = 0;
static GLuint m_vertexBuffer = 0;
static GLuint m_colorBuffer = 0;
static GLuint m_texCoordBuffer = 0;
static GLuint m_indexBuffer = 0;
static size_t m_numTris = 0;
static mat4 m_projection;

void Init() {
	LoadProgram();

	// Create sphere
	std::vector<float3> m_positions;
	std::vector<float3> m_normals;
	std::vector<float2> m_texcoords;
	std::vector<int32_t> m_indices;

	const int32_t stacks = 10;
	const int32_t slices = 10;
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
	GL_TRY(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr));
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

	LoadProjection(&m_projection, 60.0f * kDeg2Rad, 16.0f / 9.0f, 0.3f, 1000.0f);
}

void Tick() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	// input

	// bind camera uniform

	// bind program
	BindProgram();

	// set mvp matrix
	mat4 model;
	LoadTranslation(&model, float3{0.0f, 0.0f, 10.0f});
	//mat4 translate = math::TranslationMatrix(-m_player->GetPosition());
	//mat4 rotate = math::Transpose(math::mat4(m_player->GetRotation()));
	//mat4 view = rotate * translate;
	//mat4 mvp = m_projection * view * model;
	mat4 mvp = Mul(m_projection, model);
	UploadMVPMatrix(mvp);

	// render sphere

	GL_TRY(glBindVertexArray(m_vertexArray));
	GL_TRY(glDrawElements(GL_TRIANGLES, (GLsizei)(m_numTris * 3), GL_UNSIGNED_INT, 0));
	GL_TRY(glBindVertexArray(0));
}

void Destroy() {
	// TODO
}
