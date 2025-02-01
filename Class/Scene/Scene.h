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
	static AREA areaNo_;

	// ステージ
	static STAGE stageNo_;

	// クリアしたかどうか（クリアフラグ）
	static int isClear_;

	// ゲームオーバーになったか（ゲームオーバーフラグ）
	static int isGameOver_;

	// 配置する準備ができたかどうか（配置準備フラグ）
	static int isPutPreparation_;
};