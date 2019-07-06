//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#pragma once

#include "Model.h"

class SphereModel : public Model
{
public:
	SphereModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 color = glm::vec3(1.0f, 0.05f, 0.05f));
    virtual ~SphereModel(void);

    virtual void Update(float dt);
    virtual void Draw();

	virtual void Load(ci_istringstream& iss);

	virtual void Initialize();
    
protected:
    virtual bool ParseLine(const std::vector<ci_string> &token);

	glm::vec3 mColor;

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


