#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <cstdio>
#include <cmath>
#include <string>
#include <codecvt>
#include <locale>

#include "ShaderProgram.h"
#include "GameData.h"
#include "Board.h"
#include "Model.h"
#include "Texture.h"


void errorCallback(int, const char*);
void keyCallback(GLFWwindow*, int, int, int, int);
void windowSizeCallback(GLFWwindow*, int, int);
void dropCallback(GLFWwindow*, int, const char*[]);
GLFWwindow* initGLFWwindow();
void initOpenGLProgram(GLFWwindow*);
void freeOpenGLProgram(GLFWwindow*);
void drawScene(GLFWwindow*);
void updateVMatrix(float);
void askForFile(GLFWwindow*);
void loadFile(const std::string&);
std::string utf8ToString(const std::string&, const std::locale& = std::locale(""));


constexpr int
INITIAL_WIDTH = 500,
INITIAL_HEIGHT = 500;

constexpr float
PI = static_cast<float>(M_PI),
PI2 = 2.0f * PI,
FOV = 50.0f * PI / 180.0f,
Z_NEAR = 0.01f,
Z_FAR = 50.0f,
SPEED = 5.0f,
SPEED_ROT = PI / 2.0f,
MIN_DISTANCE = 3.0f,
MAX_DISTANICE = 20.0f,
MIN_VERTICAL_ANGLE = PI / 12.0f,
MAX_VERTICAL_ANGLE = PI / 2.0f;

constexpr glm::mat4 unitMatrix(1.0f);

glm::mat4 P, V, M, depthP, depthV[LIGHT_COUNT];
glm::vec4 light[LIGHT_COUNT] = {
	{8.0f, 8.0f, 0.0f, 1.0f},
	{-8.0f, 8.0f, 0.0f, 1.0f}
};
GameData *data;
Board *board;
bool paused = true;

float
speed = 0.0f,
speedVertical = 0.0f,
speedHorizontal = 0.0f;

int
currentWidth = INITIAL_WIDTH,
currentHeight = INITIAL_HEIGHT;


int main()
{
	GLFWwindow *window = initGLFWwindow();
	if (window == nullptr)
		return -1;
	initOpenGLProgram(window);

	glfwSetTime(0.0);
	while (!glfwWindowShouldClose(window)) {
		if (data) {
			drawScene(window);
		}
		else {
			askForFile(window);
		}
	}

	if (data) delete data;
	if (board) delete board;
	freeOpenGLProgram(window);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void errorCallback(int error, const char *description)
{
	fputs(description, stderr);
}


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	const Move *move;
	switch (action) {
	case GLFW_PRESS:
		switch (key) {
		case GLFW_KEY_W:
			speedVertical = SPEED_ROT;
			break;
		case GLFW_KEY_S:
			speedVertical = -SPEED_ROT;
			break;
		case GLFW_KEY_A:
			speedHorizontal = SPEED_ROT;
			break;
		case GLFW_KEY_D:
			speedHorizontal = -SPEED_ROT;
			break;
		case GLFW_KEY_Z:
			speed = SPEED;
			break;
		case GLFW_KEY_X:
			speed = -SPEED;
			break;
		case GLFW_KEY_SPACE:
			paused = !paused;
			break;
		case GLFW_KEY_LEFT:
			paused = true;
			board->finishAnimations();
			move = data->prevMove();
			if (move) {
				printf("%s\n", std::string(*move).c_str());
				board->undoMove(move);
			}
			break;
		case GLFW_KEY_RIGHT:
			paused = true;
			board->finishAnimations();
			move = data->nextMove();
			if (move) {
				printf("%s\n", std::string(*move).c_str());
				board->applyMove(move);
				board->finishAnimations();
			}
			break;
		case GLFW_KEY_ESCAPE:
			delete board;
			delete data;
			board = nullptr;
			data = nullptr;
			break;
		}
		break;
	case GLFW_RELEASE:
		switch (key) {
		case GLFW_KEY_W:
		case GLFW_KEY_S:
			speedVertical = 0;
			break;
		case GLFW_KEY_A:
		case GLFW_KEY_D:
			speedHorizontal = 0;
			break;
		case GLFW_KEY_Z:
		case GLFW_KEY_X:
			speed = 0;
			break;
		}
		break;
	}
}


void dropCallback(GLFWwindow *window, int count, const char *paths[])
{
	loadFile(utf8ToString(std::string(paths[0])));
}


void windowSizeCallback(GLFWwindow *window, int width, int height)
{
	currentWidth = width;
	currentHeight = height;
	if (height == 0) return;
	P = glm::perspective(FOV, static_cast<float>(width) / height, Z_NEAR, Z_FAR);
	glViewport(0, 0, width, height);
}


GLFWwindow *initGLFWwindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		return nullptr;
	}

	glfwSetErrorCallback(errorCallback);

	auto *window = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "Chess", nullptr, nullptr);

	if (window == nullptr) {
		fprintf(stderr, "Nie mo¿na utworzyæ okna.\n");
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW.\n");
		return nullptr;
	}

	return window;
}


void initOpenGLProgram(GLFWwindow *window)
{
	ShaderProgram::loadShaders();
	Model::loadModels();
	Texture::loadTextures();
	Object::loadObjects();
	P = glm::perspective(FOV, static_cast<float>(INITIAL_WIDTH) / INITIAL_HEIGHT, Z_NEAR, Z_FAR);
	updateVMatrix(0.0f);
	M = glm::scale(unitMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
	depthP = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -30.0f, 30.0f);
	for (size_t i = 0; i < LIGHT_COUNT; ++i)
		depthV[i] = glm::lookAt(glm::vec3(light[i]), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);
	glfwSetDropCallback(window, dropCallback);
}


void freeOpenGLProgram(GLFWwindow *window)
{
	Object::deleteObjects();
	Texture::deleteTextures();
	Model::deleteModels();
	ShaderProgram::deleteShaders();
}


void drawScene(GLFWwindow *window)
{
	float time = static_cast<float>(glfwGetTime());
	glfwSetTime(0.0);
	updateVMatrix(time);

	if (!paused) {
		board->addTime(time);
		if (board->finished()) {
			const Move *move = data->nextMove();
			if (move) {
				printf("%s\n", std::string(*move).c_str());
				board->finishAnimations();
				board->applyMove(move);
			}
		}
		board->applyAnimations();
	}

	glCullFace(GL_FRONT);
	for (size_t i = 0; i < LIGHT_COUNT; ++i) {
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, Texture::shadowMapFBO[i]);
		ShaderProgram::depthShader->use();
		glClear(GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(ShaderProgram::depthShader->getUniform("depthP"), 1, GL_FALSE, glm::value_ptr(depthP));
		glUniformMatrix4fv(ShaderProgram::depthShader->getUniform("depthV"), 1, GL_FALSE, glm::value_ptr(depthV[i]));
		board->renderShadow(M);
	}

	glViewport(0, 0, currentWidth, currentHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ShaderProgram::objectShader->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	glUniform4fv(ShaderProgram::objectShader->getUniform("light"), LIGHT_COUNT, glm::value_ptr(light[0]));
	glUniformMatrix4fv(ShaderProgram::objectShader->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P));
	glUniformMatrix4fv(ShaderProgram::objectShader->getUniform("V"), 1, GL_FALSE, glm::value_ptr(V));
	glUniformMatrix4fv(ShaderProgram::objectShader->getUniform("depthP"), 1, GL_FALSE, glm::value_ptr(depthP));
	glUniformMatrix4fv(ShaderProgram::objectShader->getUniform("depthV"), LIGHT_COUNT, GL_FALSE, glm::value_ptr(depthV[0]));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture::shadowMap[0]);
	glUniform1i(ShaderProgram::objectShader->getUniform("shadowMap[0]"), 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, Texture::shadowMap[1]);
	glUniform1i(ShaderProgram::objectShader->getUniform("shadowMap[1]"), 2);
	board->render(M);

	glfwSwapBuffers(window);
	glfwPollEvents();
}


void updateVMatrix(float time)
{
	static float distance = 15.0f,
		angleVertical = PI / 4.0f,
		angleHorizontal = PI;

	distance += speed * time;
	if (distance < MIN_DISTANCE) distance = MIN_DISTANCE;
	else if (distance > MAX_DISTANICE) distance = MAX_DISTANICE;

	angleHorizontal += speedHorizontal * time;
	if (angleHorizontal >= PI2) angleHorizontal -= PI2;
	else if (angleHorizontal <= -PI2) angleHorizontal += PI2;

	angleVertical += speedVertical * time;
	if (angleVertical > MAX_VERTICAL_ANGLE) angleVertical = MAX_VERTICAL_ANGLE;
	else if (angleVertical < MIN_VERTICAL_ANGLE) angleVertical = MIN_VERTICAL_ANGLE;

	V = glm::translate(unitMatrix, glm::vec3(0.0f, 0.0f, -distance));
	V = glm::rotate(V, angleVertical, glm::vec3(1.0f, 0.0f, 0.0f));
	V = glm::rotate(V, angleHorizontal, glm::vec3(0.0f, 1.0f, 0.0f));
}


void askForFile(GLFWwindow *window)
{
	static float vertices[] = {
		-1.0f, -1.0f,	1.0f, 1.0f,		-1.0f, 1.0f,
		-1.0f, -1.0f,	1.0f, 1.0f,		1.0f, -1.0f
	}, uvs[] = {
		0.0f, 1.0f,		1.0f, 0.0f,		0.0f, 0.0f,
		0.0f, 1.0f,		1.0f, 0.0f,		1.0f, 1.0f
	};
	static GLsizei count = 6;

	ShaderProgram::uiShader->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture::draganddrop);
	glUniform1i(ShaderProgram::uiShader->getUniform("texSampler0"), 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, uvs);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glfwSwapBuffers(window);
	glfwPollEvents();
}


void loadFile(const std::string &filename)
{
	if (data) delete data;
	if (board) delete board; 
	std::ifstream file(filename);
	data = new GameData(file);
	board = new Board;
	file.close();
}


std::string utf8ToString(const std::string& str, const std::locale &loc)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	std::wstring wstr = wconv.from_bytes(str);
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	return std::string(buf.data(), buf.size());
}