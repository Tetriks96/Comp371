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
	vector<SphereModel*> sphereModels,
	vector<BubbleGroup*> bubbleGroups)
{
	Renderer::BeginFrame();

	// Set shader to use
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");

	// Send the view projection constants to the shader
	mat4 VP = camera[currentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	// Draw models
	for (vector<SphereModel*>::iterator it = sphereModels.begin(); it < sphereModels.end(); ++it)
	{
		if ((*it)->GetVolume() > 0.0f)
		{
			(*it)->Draw();
		}
	}

	for (vector<BubbleGroup*>::iterator it = bubbleGroups.begin(); it < bubbleGroups.end(); ++it)
	{
		(*it)->Draw();
	}

	// Draw Path Lines

	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_PATH_LINES);
	glUseProgram(Renderer::GetShaderProgramID());

	// Send the view projection constants to the shader
	VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	Renderer::CheckForErrors();

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);

	Renderer::EndFrame();
}