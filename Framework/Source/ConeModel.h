#pragma once

#include "Model.h"
#include <GL\glew.h>

class ConeModel : public Model
{
public:
	ConeModel(glm::vec3 position, float radius, glm::vec3 color, glm::vec3 rotationAxis, float angle);
	virtual ~ConeModel(void);

	virtual void Draw();

	float GetRadius();

	void SetVolume(float volume);

	float GetVolume() const { return mVolume; }

protected:
	virtual void Initialize();

	glm::vec3 mColor;
	float mVolume;
	float mRadius;

private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int numOfVertices;
};

