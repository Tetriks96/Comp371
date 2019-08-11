#include "Menu.h"
#include "Renderer.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

#include <FreeImageIO.h>


using namespace std;
using namespace glm;

Menu::Menu()
{
#if defined(PLATFORM_OSX)
	mTitleTextureID = loadTexture("Textures/NeonTitle.png");
	mPressTextureID = loadTexture("Textures/NeonPress.png");
#else
	mTitleTextureID = loadTexture("../Assets/Textures/NeonTitle.png");
	mPressTextureID = loadTexture("../Assets/Textures/NeonPress.png");
#endif

	// Define and upload geometry to the GPU here ...
	createVertexBufferObject();
}

Menu::~Menu()
{

}

void Menu::Draw()
{
	Renderer::SetShader(SHADER_MENU);
	int shaderProgram = Renderer::GetShaderProgramID();

	glDisable(GL_DEPTH_TEST);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedSquareVertexArray), texturedSquareVertexArray, GL_STATIC_DRAW);

	// White background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	mat4 world(1.0f);

	setWorldMatrix(shaderProgram, world);
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	GLuint textureLocation = glGetUniformLocation(shaderProgram, "textureSampler");
	glBindTexture(GL_TEXTURE_2D, mTitleTextureID);
	glUniform1i(textureLocation, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);


Menu::~Menu()
{

}

void Menu::Draw()
{
	Renderer::SetShader(SHADER_MENU);
	int shaderProgram = Renderer::GetShaderProgramID();

	glDisable(GL_DEPTH_TEST);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedSquareVertexArray), texturedSquareVertexArray, GL_STATIC_DRAW);

	// White background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	mat4 world(1.0f);

	setWorldMatrix(shaderProgram, world);
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	GLuint textureLocation = glGetUniformLocation(shaderProgram, "textureSampler");
	glBindTexture(GL_TEXTURE_2D, mTitleTextureID);
	glUniform1i(textureLocation, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindTexture(GL_TEXTURE_2D, mTitleTextureID);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindTexture(GL_TEXTURE_2D, mPressTextureID);
	mat4 pressworld = translate(mat4(1.0f), vec3(0.0f, -1.25f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 0.5f, 1.0f));
	setWorldMatrix(shaderProgram, pressworld);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glEnable(GL_DEPTH_TEST);
	Renderer::SetShader(SHADER_SOLID_COLOR);
}

bool Menu::isPaused()
{
	return paused;
}

void Menu::Pause()
{
	paused = true;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Menu::Resume()
{
	paused = false;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int Menu::loadTexture(char* imagepath)
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

void Menu::createVertexBufferObject()
{
	// Create a vertex array
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedSquareVertexArray), texturedSquareVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(MenuVertex), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(MenuVertex),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(MenuVertex),
		(void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);
}

GLuint Menu::compileAndLinkShaders()
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
	GLuint shaderProgram = glCreateProgram();
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

const char* Menu::getVertexShaderSource()
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

void Menu::setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

const char* Menu::getFragmentShaderSource()
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




