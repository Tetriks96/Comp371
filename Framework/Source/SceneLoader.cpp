#include "SceneLoader.h"
#include "ParsingHelper.h"
#include "CubeModel.h"
#include "SphereModel.h"

using namespace std;

void SceneLoader::LoadScene(
	const char * scene_path,
	std::vector<Model*>* model,
	std::vector<Animation*>* animation,
	std::vector<AnimationKey*>* animationKey)
{
	// Using case-insensitive strings and streams for easier parsing
	ci_ifstream input;
	input.open(scene_path, ios::in);

	// Invalid file
	if (input.fail())
	{
		fprintf(stderr, "Error loading file: %s\n", scene_path);
		getchar();
		exit(-1);
	}

	ci_string item;
	while (std::getline(input, item, '['))
	{
		ci_istringstream iss(item);

		ci_string result;
		if (std::getline(iss, result, ']'))
		{
			if (result.empty() == false && result[0] == '#')
			{
				// this is a comment line
			}
			else if (result == "cube")
			{
				// Box attributes
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				model->push_back(cube);
			}
			else if (result == "sphere")
			{
				SphereModel* sphere = new SphereModel();
				sphere->Load(iss);
				model->push_back(sphere);
			}
			else if (result == "animationkey")
			{
				AnimationKey* key = new AnimationKey();
				key->Load(iss);
				animationKey->push_back(key);
			}
			else if (result == "animation")
			{
				Animation* anim = new Animation();
				anim->Load(iss);
				animation->push_back(anim);
			}
			else
			{
				fprintf(stderr, "Error loading scene file... !");
				getchar();
				exit(-1);
			}
		}
	}
	input.close();

	// Set Animation vertex buffers
	for (vector<Animation*>::iterator it = animation->begin(); it < animation->end(); ++it)
	{
		// Draw model
		(*it)->CreateVertexBuffer();
	}
}