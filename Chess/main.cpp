#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <cmath>

#include "ShaderProgram.h"
#include "GameData.h"
#include "Board.h"
#include "Model.h"


void errorCallback(int, const char*);
void keyCallback(GLFWwindow*, int, int, int, int);
void windowSizeCallback(GLFWwindow*, int, int);
GLFWwindow* initGLFWwindow();
void initOpenGLProgram(GLFWwindow*);
void freeOpenGLProgram(GLFWwindow*);
void drawScene(GLFWwindow*);
void updateVMatrix(float);


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
SPEED_ROT = PI / 2.0f;

constexpr glm::mat4 unitMatrix(1.0f);


ShaderProgram *shaderProgram;
glm::mat4 P, V, M;
GameData *data;
Board *board;

float
speed = 0.0f,
speedVertical = 0.0f,
speedHorizontal = 0.0f;


GLuint VertexArrayID;
GLuint vertexbuffer;


int main()
{
	GLFWwindow *window = initGLFWwindow();
	if (window == nullptr)
		return -1;
	initOpenGLProgram(window);

	data = new GameData("test.pgn");
	board = new Board(shaderProgram, P, V, M);
	while (!glfwWindowShouldClose(window)) {
		drawScene(window);
	}

	delete data;
	delete board;
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


void windowSizeCallback(GLFWwindow *window, int width, int height)
{
	if (height == 0) return;
	P = glm::perspective(FOV, static_cast<float>(INITIAL_WIDTH) / INITIAL_HEIGHT, Z_NEAR, Z_FAR);
	glViewport(0, 0, width, height);
}


GLFWwindow *initGLFWwindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		return nullptr;
	}

	glfwSetErrorCallback(errorCallback);

	auto *window = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "OpenGL", nullptr, nullptr);

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
	shaderProgram = new ShaderProgram("vertex.glsl", "fragment.glsl");
	Model::loadModels();
	P = glm::perspective(FOV, static_cast<float>(INITIAL_WIDTH) / INITIAL_HEIGHT, Z_NEAR, Z_FAR);
	updateVMatrix(0.0f);
	M = unitMatrix;

	glClearColor(.5f, .5f, .5f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	shaderProgram->use();
}


void freeOpenGLProgram(GLFWwindow *window)
{
	delete shaderProgram;
}


void drawScene(GLFWwindow *window)
{
	static float totalTime = 0.0f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float time = static_cast<float>(glfwGetTime());
	updateVMatrix(time);
	totalTime += time;
	if (totalTime >= 0.1f) {
		totalTime -= 0.1f;
		const Move *move = data->nextMove();
		if (move) {
			printf("%s\n", std::string(*move).c_str());
			board->applyMove(move);
		}
	}

	glfwSetTime(0);

	board->render();

	glfwSwapBuffers(window);
	glfwPollEvents();
}


void updateVMatrix(float time)
{
	static float distance = 15.0f,
		angleVertical = PI / 4.0f,
		angleHorizontal = 0.0f;

	distance += speed * time;
	if (distance <= 1.0f) distance = 1.0f;

	angleHorizontal += speedHorizontal * time;
	if (angleHorizontal >= PI2) angleHorizontal -= PI2;
	if (angleHorizontal <= -PI2) angleHorizontal += PI2;

	angleVertical += speedVertical * time;
	if (angleVertical >= PI2) angleVertical -= PI2;
	if (angleVertical <= -PI2) angleVertical += PI2;

	V = glm::translate(unitMatrix, glm::vec3(0.0f, 0.0f, -distance));
	V = glm::rotate(V, angleVertical, glm::vec3(1.0f, 0.0f, 0.0f));
	V = glm::rotate(V, angleHorizontal, glm::vec3(0.0f, 1.0f, 0.0f));
}