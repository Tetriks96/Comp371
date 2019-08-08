#include "SpikeBall.h"
#include "Renderer.h"
#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <GL\glew.h>

using namespace glm;

SpikeBall::SpikeBall(vec3 position, float volume, vec3 color)
{
	mBubble = new Bubble(position, volume, color);

	/*int sides = 8;
	int conePerSides = 3;
	float radius = mBubble->GetRadius();
	// Top cone
	ConeModel* topCone = new ConeModel(vec3(position.x, position.y, position.z + radius), radius, color, vec3(1.0f), 0.0f);
	mCones.push_back(topCone);
	// Bottom cone
	ConeModel* bottomCone = new ConeModel(vec3(position.x, position.y, position.z + radius), radius, color, vec3(0.0, 1.0, 0.0), 180.0f);
	mCones.push_back(bottomCone);

	for (int i = 0; i < sides; i++) {
		for (int j = 0; j < conePerSides; j++) {
			float twoPi = 2.0 * (float)M_PI;
			float newX = position.x + radius * cos(twoPi * i / sides);
			float newY = position.y + radius * sin(twoPi * i / sides);
			float r = sqrt(pow(newX, 2.0) + pow(newY, 2.0));
			float newZ = position.z + r * sin(((float)M_PI / 2) - twoPi * (i + 1) / sides);
			vec3 newPos = vec3(newX, newY, newZ);
			ConeModel* cone = new ConeModel(newPos, radius, vColor);
			mCones.push_back(cone);
		}
	}
	*/
}

SpikeBall::~SpikeBall()
{
	delete mBubble;
	
	for (std::vector<ConeModel*>::iterator it = mCones.begin(); it < mCones.end(); ++it)
	{
		delete *it;
	}

	mCones.clear();
}

void SpikeBall::Draw()
{
	mBubble->Draw();
	for (std::vector<ConeModel*>::iterator it = mCones.begin(); it < mCones.end(); ++it)
	{
		if (mBubble->GetVolume() > 0.0f)
		{
			(*it)->Draw();
		}
	}
}