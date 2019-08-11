#pragma once
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Languag
#include <GL/glew.h>
class Menu {
public:
	Menu();
	~Menu();
	void Draw();
	bool isPaused();
	void Pause();
	void Resume();
	static int loadTexture(char* imagepath);
	void createVertexBufferObject();
	static GLuint compileAndLinkShaders();
	static const char* getFragmentShaderSource();
	static const char* getVertexShaderSource();
	void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix);

protected:
	bool paused = false;
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

	GLuint mTitleTextureID;
	GLuint mPressTextureID;
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int numOfVertices;
 };