#include "PlayerBubbleGroup.h"

using namespace glm;

PlayerBubbleGroup::PlayerBubbleGroup()
{
	SphereModel* initialBubble = new SphereModel(vec3(0.0f), 2.0f, vec3(0.0f, 1.0f, 1.0f));
	mSphereModels.push_back(initialBubble);
}

PlayerBubbleGroup::~PlayerBubbleGroup()
{

}

void PlayerBubbleGroup::Update()
{

}