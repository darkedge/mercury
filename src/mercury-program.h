#pragma once

void LoadPrograms();

// Geometry program
void BindGeometryProgram();
void SetGeometryProgramConstants(const mat4 &mvp, const mat4 &m);

// Image-based lighting program
void BindIBLProgram();
void SetIBLProgramConstants(GLint r0, GLint r1, GLint r2, GLint r3, const mat4 &invView, const mat4 &invProj);
