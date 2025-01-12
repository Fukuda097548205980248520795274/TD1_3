#pragma once
#include <Novice.h>
#include "Enumeration.h"

class Scene
{
public:

	/// <summary>
	/// 画面を切り替える
	/// </summary>
	/// <param name="keys">キー</param>
	/// <param name="preKeys">前のキー</param>
	static void Switch(const char* keys, const char* preKeys);

	// シーン
	static SCENE sceneNo_;

	// ゲーム全体のフレーム
	static int gameFrame_;

	// 操作できるかどうか（操作フラグ）
	static int isOperation_;
};