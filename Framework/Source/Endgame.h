#pragma once

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Languag
class Endgame {
public:
	void Draw();
	int loadTexture(char* imagepath);
	int createVertexBufferObject();
	int compileAndLinkShaders();
	const char* getFragmentShaderSource();
	const char* getVertexShaderSource();
	void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix);
	bool getLoss();
	void setLoss(bool state);
	bool getWin();
	void setWin(bool state);

private:

	struct EndgameVertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 textureCoordinate;

	};

	const EndgameVertex texturedSquareVertexArray[4] = {
		glm::vec3(-0.75f, -0.125f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f),
		glm::vec3(0.75f, -0.125f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
		glm::vec3(0.75f, 0.25f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f),
		glm::vec3(-0.75f, 0.25f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f),
	};
	bool stateLoss = false;
	bool stateWin = false;
};