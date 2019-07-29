#include "BubbleGroup.h"

using namespace std;

void BubbleGroup::Draw()
{
	for (vector<SphereModel*>::iterator it = mSphereModels.begin(); it < mSphereModels.end(); ++it)
	{
		(*it)->Draw();
	}
}