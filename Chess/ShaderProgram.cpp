#include "ShaderProgram.h"

#include <cstdio>


const ShaderProgram *ShaderProgram::objectShader, *ShaderProgram::uiShader;


ShaderProgram::ShaderProgram(const char *vertexFilename, const char *fragmentFilename, const char *geometryFilename)
{
	program = glCreateProgram();

	printf("Loading vertex shader...\n");
	vertex = loadShader(GL_VERTEX_SHADER, vertexFilename);
	glAttachShader(program, vertex);

	printf("Loading fragment shader...\n");
	fragment = loadShader(GL_FRAGMENT_SHADER, fragmentFilename);
	glAttachShader(program, fragment);

	if (geometryFilename != nullptr) {
		printf("Loading geometry shader...\n");
		geometry = loadShader(GL_GEOMETRY_SHADER, geometryFilename);
		glAttachShader(program, geometry);
	}
	else geometry = 0;

	glLinkProgram(program);

	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	printf("Shader program created \n");
}


ShaderProgram::~ShaderProgram()
{
	glDetachShader(program, vertex);
	glDeleteShader(vertex);

	glDetachShader(program, fragment);
	glDeleteShader(fragment);

	if (geometry != 0) {
		glDetachShader(program, geometry);
		glDeleteShader(geometry);
	}

	glDeleteProgram(program);
}


void ShaderProgram::loadShaders()
{
	objectShader = new ShaderProgram("phongVertex.glsl", "phongFragment.glsl");
	uiShader = new ShaderProgram("uiVertex.glsl", "uiFragment.glsl");
}


void ShaderProgram::deleteShaders()
{
	delete objectShader;
	delete uiShader;
}


const char* ShaderProgram::readFile(const char *filename)
{
	int filesize;
	FILE *file;
	char *result;

	fopen_s(&file, filename, "rb");
	if (file == nullptr)
		return nullptr;

	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);
	result = new char[filesize + 1];

	fread_s(result, filesize, 1, filesize, file);
	result[filesize] = 0;
	fclose(file);

	return result;
}


GLuint ShaderProgram::loadShader(GLenum type, const char *filename)
{
	GLuint shader = glCreateShader(type);
	const GLchar *shaderSource = readFile(filename);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);
	delete[] shaderSource;

	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[]infoLog;
	}

	return shader;
}