#include "SceneLoader.h"
#include "ParsingHelper.h"
#include "SphereModel.h"

using namespace std;

void SceneLoader::LoadScene(
	const char * scene_path,
	World* world,
	std::vector<Model*>* model)
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
			else if (result == "world")
			{
				world->Load(iss);
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
}