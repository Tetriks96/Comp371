#include "SpikeBall.h"
#include "Renderer.h"
#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <GL\glew.h>

using namespace std;
using namespace glm;

SpikeBall::SpikeBall(vec3 position, float radius, vec3 color) : Model(position, vec3(radius), vec3(0.0f, 1.0f, 0.0f), 0.0f)
{
	mSphereModel = new SphereModel(position, radius, color);
	mConeModel = new ConeModel(vec3(0.0f), 0.5f * radius, 0.2f * radius, color, vec3(0.0f, 1.0f, 0.0f), 0.0f);
}

SpikeBall::~SpikeBall()
{
	delete mSphereModel;
	delete mConeModel;
}

void SpikeBall::Draw()
{
	mSphereModel->Draw();

	// Scaling of sphere x = y = z = radius. Any will do.
	float sphereRadius = mSphereModel->GetScaling().x;

	// x : x coordinate of rotation axis
	// y : y coordinate of rotation axis
	// z : angular distance from z axis in degrees
	vector<vec3> coneInfos {
		vec3(1.0f,  0.0f, 0.0f),
		vec3(1.0f,  0.0f, 90.0f),
		vec3(1.0f,  0.0f, 180.0f),
		vec3(1.0f,  0.0f, -90.0f),
		vec3(0.0f,  1.0f, 90.0f),
		vec3(0.0f,  1.0f, -90.0f),
		// TODO
		// ...
	};

	for (int i = 0; i < (int)coneInfos.size(); i++)
	{
		vec3 coneInfo = coneInfos[i];

		vec3 coneRotationAxis = vec3(coneInfo.x, coneInfo.y, 0.0f);

		mConeModel->SetRotation(coneRotationAxis, coneInfo.z);

		// TODO: Adjust the constant until the cone's base connects with the sphere correctly
		float constant = 1.0f;

		vec3 unrotatedConeTranslation = constant * sphereRadius * vec3(0.0f, 0.0f, 1.0f);

		vec3 rotatedConeTranslation = vec3(rotate(mat4(1.0f), radians(coneInfo.z), coneRotationAxis) * vec4(unrotatedConeTranslation, 1.0f));

		mConeModel->SetPosition(GetPosition() + rotatedConeTranslation);

		mConeModel->Draw();
	}
}