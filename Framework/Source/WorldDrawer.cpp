#include "WorldDrawer.h"
#include "Renderer.h"
#include <glm/glm.hpp>

#include "Camera.h"
#include "Model.h"
#include "Animation.h"
#include "ControllableSphere.h"

using namespace glm;
using namespace std;

void WorldDrawer::DrawWorld(
	vector<Camera*> camera,
	unsigned int currentCamera,
	vector<Model*> model,
	vector<Animation*> animation,
	vector<AnimationKey*> animationKey,
	vector<ControllableSphere*> sphere)
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
	for (vector<Model*>::iterator it = model.begin(); it < model.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			(*it)->Draw();
		}
	}

	for (vector<ControllableSphere*>::iterator it = sphere.begin(); it < sphere.end(); ++it)
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

	for (vector<Animation*>::iterator it = animation.begin(); it < animation.end(); ++it)
	{
		(*it)->Draw();
	}

	for (vector<AnimationKey*>::iterator it = animationKey.begin(); it < animationKey.end(); ++it)
	{
		mat4 VP = camera[currentCamera]->GetViewProjectionMatrix();
		glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

		(*it)->Draw();
	}

	Renderer::CheckForErrors();

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);

	Renderer::EndFrame();
}