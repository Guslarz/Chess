#pragma once

#include <GL/glew.h>


class ShaderProgram
{
	GLuint vertex, fragment, geometry, program;

public:
	ShaderProgram(const char*, const char*, const char* = nullptr);
	~ShaderProgram();

	void use() const { glUseProgram(program); }
	GLuint getUniform(const char *name) const { return glGetUniformLocation(program, name); }
	GLuint getAttrib(const char *name) const { return glGetAttribLocation(program, name); }

	static const ShaderProgram *objectShader, *uiShader;

	static void loadShaders();
	static void deleteShaders();

private:
	static const char* readFile(const char*);
	static GLuint loadShader(GLenum, const char*);
};

