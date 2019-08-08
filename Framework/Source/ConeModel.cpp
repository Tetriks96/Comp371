#include "ConeModel.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <GL\glew.h>

using namespace glm;

ConeModel::ConeModel(glm::vec3 position, float height, float radius, glm::vec3 color, glm::vec3 rotationAxis, float angle)
	: Model(position, vec3(radius, radius, height), rotationAxis, angle)
{
	mColor = color;
	mRadius = radius;
	mHeight = height;
	Initialize();
}

ConeModel::~ConeModel()
{
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);
}

void ConeModel::Draw()
{
	// Draw the Vertex Buffer
	// Note this draws a Sphere
	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLE_FAN, 0, numOfVertices);
}

float ConeModel::GetRadius()
{
	return mRadius;
}

void ConeModel::Initialize()
{
	const int numberOfSlices = 200;
	const int radius = 5;
	GLfloat twoPi = 2.0f * (float)M_PI;

	vec3 newPos = vec3(0.0, 0.0, 1.0);

	Vertex vertexBuffer[numberOfSlices + 1];
		// position,    normal,      color
	vertexBuffer[0] = { newPos, newPos, mColor };

	for (int i = 1; i < numberOfSlices; i++) {
		float mX = (radius * cos(twoPi * i / numberOfSlices)) / radius;
		float mY = (radius * sin(twoPi * i / numberOfSlices)) / radius;
		float mZ = 0.0;
		vec3 mConeVertice = vec3(mX, mY, mZ);
		vertexBuffer[i] = { mConeVertice, mConeVertice, mColor };
	}
	
	float mX = (radius * cos(twoPi * 1 / numberOfSlices)) / radius;
	float mY = (radius * sin(twoPi * 1 / numberOfSlices)) / radius;
	float mZ = 0.0;
	vec3 mConeVertice = vec3(mX, mY, mZ);
	vertexBuffer[numberOfSlices] = { mConeVertice, mConeVertice, mColor };

	numOfVertices = sizeof(vertexBuffer) / sizeof(Vertex);

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);

	// 1st attribute buffer : vertex Positions
	glVertexAttribPointer(0,              // attribute. No particular reason for 0, but must match the layout in the shader.
		3,              // size
		GL_FLOAT,       // type
		GL_FALSE,       // normalized?
		sizeof(Vertex), // stride
		(void*)0        // array buffer offset
	);
	glEnableVertexAttribArray(0);

	// 2nd attribute buffer : vertex normal
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
	);
	glEnableVertexAttribArray(1);


	// 3rd attribute buffer : vertex color
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2 * sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
	);
	glEnableVertexAttribArray(2);
}