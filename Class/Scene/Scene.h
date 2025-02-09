#pragma once
#include <Novice.h>
#include "Enumeration.h"
#include "./Class/Map/Map.h"

class Scene
{
public:

	// シーン
	static SCENE sceneNo_;

	// エリア
	static int areaNo_;

	// ステージ
	static int stageNo_;

	// クリアしたかどうか（クリアフラグ）
	static int isClear_;

	// ゲームオーバーになったか（ゲームオーバーフラグ）
	static int isGameOver_;

	// クリア後の操作
	static CLEAR clearNo_;

	// ゲームオーバー後の操作
	static GAMEOVER gameoverNo_;

	// ポーズの操作
	static POSE poseNo_;

	// 配置する準備ができたかどうか（配置準備フラグ）
	static int isPutPreparation_;
};