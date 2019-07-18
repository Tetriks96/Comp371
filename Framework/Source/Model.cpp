//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "Model.h"
#include "World.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace std;
using namespace glm;

Model::Model(vec3 position, vec3 scaling, vec3 rotationAxis, float rotationAngleInDegrees)
{
	mPosition = position;
	mScaling = scaling;
	mRotationAxis = rotationAxis;
	mRotationAngleInDegrees = rotationAngleInDegrees;
}

Model::~Model()
{
}

glm::mat4 Model::GetWorldMatrix() const
{
	mat4 T = translate(mat4(1.0f), mPosition);
	mat4 TR = rotate(T, radians(mRotationAngleInDegrees), mRotationAxis);
	mat4 TRS = scale(TR, mScaling);
	return TRS;
}

void Model::SetPosition(glm::vec3 position)
{
	mPosition = position;
}

void Model::SetScaling(glm::vec3 scaling)
{
	mScaling = scaling;
}

void Model::SetRotation(glm::vec3 axis, float angleDegrees)
{
	mRotationAxis = axis;
	mRotationAngleInDegrees = angleDegrees;
}
