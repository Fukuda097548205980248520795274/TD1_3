#include "Scene.h"


SCENE Scene::sceneNo_ = SCENE_START;
int Scene::gameFrame_ = 0;
int Scene::isOperation_ = false;

int Scene::isClear_ = false;
int Scene::isGameOver_ = false;

int Scene::selectStage_ = 0;
int Scene::isPutPreparation_ = false;

int Scene::isTitleIn = false;

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


	// ゲームを操作できない（操作フラグがfalseである）ときに、フレームを進める
	if (isOperation_ == false)
	{
		gameFrame_++;
	}


	// シーン切り替え
	switch (sceneNo_)
	{
	case SCENE_START:
		// スタート画面

		// タイトルが入るまで操作ができない
		if (isTitleIn == false)
		{
			if (gameFrame_ >= 180)
			{
				isOperation_ = true;

				// タイトルが入る（タイトルインフラグがtrueになる）
				isTitleIn = true;
			}
		}

		// スペースキーを押すと、ステージセレクトに移る
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
		{
			if (isOperation_)
			{
				isOperation_ = false;
			}
		}

		if (isTitleIn)
		{
			if (gameFrame_ >= 300)
			{
				isOperation_ = true;
				sceneNo_ = SCENE_STAGE;
			}
		}

		break;

	case SCENE_STAGE:
		// ステージセレクト画面
		
		if (keys[DIK_TAB] && !preKeys[DIK_TAB])
		{
			Scene::sceneNo_ = SCENE_START;
		}

		break;

	case SCENE_GAME:
		// ゲーム画面


		break;
	}
}

/// <summary>
/// 描画する
/// </summary>
void Scene::Draw()
{
	// シーン切り替え
	switch (sceneNo_)
	{
	case SCENE_START:
		// スタート画面

		

		

		break;

	case SCENE_STAGE:
		// ステージセレクト画面

		

		break;

	case SCENE_GAME:
		// ゲーム画面


		break;
	}
}