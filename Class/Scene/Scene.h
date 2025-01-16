#pragma once
#include <Novice.h>
#include "Enumeration.h"
#include "./Class/Map/Map.h"

class Scene
{
public:

	/// <summary>
	/// 画面を切り替える
	/// </summary>
	/// <param name="keys">キー</param>
	/// <param name="preKeys">前のキー</param>
	static void Switch(const char* keys, const char* preKeys);

	/// <summary>
	/// フェードアウト
	/// </summary>
	/// <param name="alpha">透明度</param>
	/// <param name="active">フラグ</param>
	static void FadeOut(int alpha_,int active_);

	/// <summary>
	/// フェードイン
	/// </summary>
	/// <param name="alpha">透明度</param>
	/// <param name="active">フラグ</param>
	 static void FadeIn(int alpha_,int active_);

	// シーン
	static SCENE sceneNo_;

	// ゲーム全体のフレーム
	static int gameFrame_;

	// 操作できるかどうか（操作フラグ）
	static int isOperation_;

	// 選んでいるステージ
	static int selectStage_;

	// 配置する準備ができたかどうか（配置準備フラグ）
	static int isPutPreparation_;

	
};