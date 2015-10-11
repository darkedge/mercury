#pragma once

void LoadPrograms();

// Geometry program
void BindGeometryProgram();
void SetGeometryProgramConstants(const mat4 &mvp, const mat4 &m);

// Image-based lighting program
void BindIBLProgram();
void SetIBLProgramConstants();
