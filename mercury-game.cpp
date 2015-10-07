#include "mercury-game.h"
#include "mercury-program.h"

void Init() {
	LoadProgram();
}

void Tick() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	// input

	// bind camera

	// bind program

	// bind texture

	// set mvp matrix

	// render sphere

#if 0
	GL_TRY(glBindVertexArray(m_vertexArray));
	GL_TRY(glDrawElements(GL_TRIANGLES, m_numTris * 3, GL_UNSIGNED_INT, 0));
	GL_TRY(glBindVertexArray(0));
#endif
}
