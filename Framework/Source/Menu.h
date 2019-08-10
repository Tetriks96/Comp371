#pragma once
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Languag
class Menu {
public:
	void Draw();
	bool isPaused();
	void toggle();
	int loadTexture(char* imagepath);
	int createVertexBufferObject();
	int compileAndLinkShaders();
	const char* getFragmentShaderSource();
	const char* getVertexShaderSource();
	void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix);

protected:
	bool paused = true;
private:

	struct MenuVertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 textureCoordinate;

	};

	const MenuVertex texturedSquareVertexArray[4] = {
		glm::vec3(-0.75f, 0.75f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f),
		glm::vec3(0.75f, 0.75f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
		glm::vec3(0.75f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f),
		glm::vec3(-0.75f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f),
	};

	int textureID;
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int numOfVertices;
 };