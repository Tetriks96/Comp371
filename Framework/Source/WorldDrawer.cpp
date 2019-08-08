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

	//Enable blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Need depth test and culling to avoid for better rendering of the spheres
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Draw Bubbles
	//set the uniform variable transparency to 1 for opaqueness
	GLuint transparencyLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "transparency");
	glUniform1f(transparencyLocation, 0.5f);

	for (vector<Bubble*>::iterator it = bubbles.begin(); it < bubbles.end(); ++it)
	{
		(*it)->Draw();
	}
	glDisable(GL_DEPTH_TEST);

	//Draw Bubble Group

	//set the transparency value desired
	glUniform1f(transparencyLocation, 0.5f);

	for (vector<BubbleGroup*>::iterator it = bubbleGroups.begin(); it < bubbleGroups.end(); ++it)
	{
		(*it)->Draw();
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

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