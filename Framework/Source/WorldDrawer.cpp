#include "WorldDrawer.h"
#include "Renderer.h"
#include <glm/glm.hpp>

#include "Camera.h"
#include "Model.h"

using namespace glm;
using namespace std;

void WorldDrawer::DrawWorld(
	vector<Camera*> camera,
	unsigned int currentCamera,
	vector<Bubble*> bubbles,
	vector<SpikeBall*> spikeBalls,
	vector<BubbleGroup*> bubbleGroups)
{

	// Set shader to use
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");

	// Send the view projection constants to the shader
	mat4 VP = camera[currentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	// Draw models
	for (vector<Bubble*>::iterator it = bubbles.begin(); it < bubbles.end(); ++it)
	{
		if (*it == nullptr)
		{
			continue;
		}
		if ((*it)->GetVolume() > 0.0f)
		{
			(*it)->Draw();
		}
	}

	for (vector<SpikeBall*>::iterator it = spikeBalls.begin(); it < spikeBalls.end(); ++it)
	{
		(*it)->Draw();
	}

	// Draw Player Bubble (BubbleGroups[0]) last, because the Player Bubble Group is Transparent
	for (int i = (int)bubbleGroups.size() - 1; i >= 0; i--)
	{
		bubbleGroups[i]->Draw();
	}

	// Send the view projection constants to the shader
	VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	Renderer::CheckForErrors();
}