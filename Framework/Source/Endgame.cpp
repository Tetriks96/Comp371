#include "Endgame.h"
#include "World.h"
#include "WorldDrawer.h"
#include "Renderer.h"
#include "EventManager.h"
#include "Model.h"

#include <iostream>
#include <list>
#include <algorithm>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include <FreeImageIO.h>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
						// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language

using namespace std;
using namespace glm;

void Endgame::Draw()
{
	glDisable(GL_DEPTH_TEST);
#if defined(PLATFORM_OSX)
	std::string texturePathPrefix = "Textures/";
#else
	std::string texturePathPrefix = "../Assets/Textures/";
#endif

	GLuint textureID;
	std::string image;
	/*if (true) {
		image = "GameOver.png";
		char* path = &(texturePathPrefix + image)[0];
		textureID = loadTexture(path);
	}
	else {
		textureID = loadTexture("../Assets/Textures");
	}*/

	if (getWin()) {
		textureID = loadTexture("../Assets/Textures/Winner.png");
	}
	if (getLoss()) {
		textureID = loadTexture("../Assets/Textures/GameOver.png");
	}

	GLuint pressTextureID = loadTexture("../Assets/Textures/PlayAgain.png");
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Compile and link shaders here ...
	int shaderProgram = compileAndLinkShaders();

	// Define and upload geometry to the GPU here ...
	int vbo = createVertexBufferObject();

	mat4 world(1.0f);
	setWorldMatrix(shaderProgram, world);
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	GLuint textureLocation = glGetUniformLocation(shaderProgram, "textureSampler");
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(textureLocation, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindTexture(GL_TEXTURE_2D, pressTextureID);
	mat4 pressworld = translate(mat4(1.0f), vec3(0.0f, -0.9f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 0.5f, 1.0f));
	setWorldMatrix(shaderProgram, pressworld);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glEnable(GL_DEPTH_TEST);
}

int Endgame::loadTexture(char* imagepath)
{
	// Load image using the Free Image library
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(imagepath, 0);
	FIBITMAP* image = FreeImage_Load(format, imagepath);
	FIBITMAP* image32bits = FreeImage_ConvertTo32Bits(image);

	// Get an available texture index from OpenGL
	GLuint texture = 0;
	glGenTextures(1, &texture);
	assert(texture != 0);

	// Set OpenGL filtering properties (bi-linear interpolation)
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Retrieve width and hight
	int width = FreeImage_GetWidth(image32bits);
	int height = FreeImage_GetHeight(image32bits);

	// This will upload the texture to the GPU memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32bits));

	// Free images
	FreeImage_Unload(image);
	FreeImage_Unload(image32bits);

	return texture;
}

int Endgame::createVertexBufferObject()
{
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedSquareVertexArray), texturedSquareVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(EndgameVertex), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(EndgameVertex),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(EndgameVertex),
		(void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);


	return vertexBufferObject;
}

int Endgame::compileAndLinkShaders()
{
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = getVertexShaderSource();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = getFragmentShaderSource();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

const char* Endgame::getVertexShaderSource()
{
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColor;"
		"layout (location = 2) in vec2 aUV;"
		"uniform mat4 worldMatrix;"
		"out vec3 vertexColor;"
		"out vec2 vertexUV;"
		"void main()"
		"{"
		" vertexColor = aColor;"
		" gl_Position = worldMatrix*vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"   vertexUV = aUV;"
		"}";
}

void Endgame::setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

bool Endgame::getLoss()
{
	return stateLoss;
}

void Endgame::setLoss(bool state)
{
	if (stateWin) return;
	stateLoss = state;
}

bool Endgame::getWin()
{
	return stateWin;
}

void Endgame::setWin(bool state)
{
	if (stateLoss) return;
	stateWin = state;
}

const char* Endgame::getFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"in vec3 vertexColor;"
		"in vec2 vertexUV;"
		"uniform sampler2D textureSampler;"
		"out vec4 FragColor;"
		"void main()"
		"{"
		"   vec4 textureColor = texture( textureSampler, vertexUV );"
		"   FragColor = textureColor * vec4(vertexColor.r, vertexColor.g, vertexColor.b, 0.1f);"

		"	if (FragColor.a <= 0.02f)"
		"		discard;"
		"}";
}