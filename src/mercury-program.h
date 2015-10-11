#pragma once

// Geometry program
void LoadGeometryProgram();
void BindGeometryProgram();
void SetGeometryProgramConstants(const mat4 &mvp, const mat4 &m);

// Image-based lighting program
void LoadIBLProgram();
void BindIBLProgram();
void SetIBLProgramConstants();
