#include "Scene.h"

/// <summary>
/// 画面を切り替える
/// </summary>
/// <param name="keys"></param>
/// <param name="preKeys"></param>
void Scene::Switch(const char* keys, const char* preKeys)
{
	// null を探す
	if (keys == nullptr || preKeys == nullptr)
	{
		return;
	}


	switch (sceneNo_)
	{
	case SCENE_START:

		break;

	case SCENE_STAGE:

		break;

	case SCENE_GAME:

		break;
	}
}