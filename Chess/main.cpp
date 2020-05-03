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

#include "ShaderProgram.h"
#include "GameData.h"
#include "Board.h"
#include "Model.h"
#include "Texture.h"


void errorCallback(int, const char*);
void keyCallback(GLFWwindow*, int, int, int, int);
void windowSizeCallback(GLFWwindow*, int, int);
GLFWwindow* initGLFWwindow();
void initOpenGLProgram(GLFWwindow*);
void freeOpenGLProgram(GLFWwindow*);
void drawScene(GLFWwindow*);
void updateVMatrix(float);
void renderSurroundings();
void askForFile(GLFWwindow*);


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


ShaderProgram *mainShader, *simpleShader;
glm::mat4 P, V, M;
GameData *data;
Board *board;
bool paused = false;

float
speed = 0.0f,
speedVertical = 0.0f,
speedHorizontal = 0.0f;


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


void windowSizeCallback(GLFWwindow *window, int width, int height)
{
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
	mainShader = new ShaderProgram("vertex.glsl", "fragment.glsl");
	simpleShader = new ShaderProgram("vertexSimple.glsl", "fragmentSimple.glsl");
	Model::loadModels();
	Texture::loadTextures();
	Object::loadObjects();
	P = glm::perspective(FOV, static_cast<float>(INITIAL_WIDTH) / INITIAL_HEIGHT, Z_NEAR, Z_FAR);
	updateVMatrix(0.0f);
	M = glm::scale(unitMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);
}


void freeOpenGLProgram(GLFWwindow *window)
{
	Object::deleteObjects();
	Texture::deleteTextures();
	Model::deleteModels();
	delete simpleShader;
	delete mainShader;
}


void drawScene(GLFWwindow *window)
{
	mainShader->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float time = static_cast<float>(glfwGetTime());
	glfwSetTime(0.0);
	updateVMatrix(time);

	glUniformMatrix4fv(mainShader->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P));
	glUniformMatrix4fv(mainShader->getUniform("V"), 1, GL_FALSE, glm::value_ptr(V));

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
	board->render(mainShader, M);
	//renderSurroundings();

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


void renderSurroundings()
{
	static Object floor(Model::floor, Texture::floor),
		walls(Model::cylinder, Texture::fog);

	simpleShader->use();

	glm::vec3 scale(25.0f, 1.0f, 25.0f);
	glUniformMatrix4fv(simpleShader->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P));
	glUniformMatrix4fv(simpleShader->getUniform("V"), 1, GL_FALSE, glm::value_ptr(V));
	glUniformMatrix4fv(simpleShader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(glm::scale(M, scale)));
	floor.render(simpleShader);

	scale.y = 25.0f;
	glUniformMatrix4fv(simpleShader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(glm::scale(M, scale)));
	walls.render(simpleShader);
}


void askForFile(GLFWwindow *window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(window);
	std::cout << "Plik PGN: ";
	std::string filename;
	std::cin >> filename;
	if (data) delete data;
	if (board) delete board;
	data = new GameData(filename);
	board = new Board;
}