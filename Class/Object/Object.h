#pragma once
#include <Novice.h>
#include "Structure.h"
#include "./Function/Matrix/Matrix.h"
#include "./Class/Map/Map.h"

class Object
{
public:

	/// <summary>
	/// マップを更新する
	/// </summary>
	/// <param name="velX">X軸の移動速度</param>
	/// <param name="velY">Y軸の移動速度</param>
	void MapUpdate(float velX , float velY);

	/// <summary>
	/// ローカル座標をワールド座標に変換する
	/// </summary>
	void LocalToScreen();

	// 図形
	struct Shape shape_;

	// 位置
	struct Pos2 pos_;

	// マップ
	struct VertexMap map_;

	// 移動速度
	struct Vec2 vel_;

	// ジャンプ
	struct Jump jump_;
};