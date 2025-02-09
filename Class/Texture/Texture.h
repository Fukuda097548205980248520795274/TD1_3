#pragma once
#include <Novice.h>
#include "Structure.h"
#include "./Function/Matrix/Matrix.h"
#include "./Class/Scene/Scene.h"

class Texture
{
public:

	/// <summary>
	/// ローカル座標をワールド座標に変換する
	/// </summary>
	void LocalToScreen();

	/// <summary>
	/// 動かす
	/// </summary>
	/// <param name="gameFrame">ゲームフレーム</param>
	virtual void Move(int gameFrame);

	/// <summary>
	/// 描画する
	/// </summary>
	/// <param name="gameFrame">ゲームフレーム</param>
	virtual void Draw(int gameFrame);

	// 図形
	struct Shape shape_;

	// 位置
	struct Pos2 pos_;

	// 移動速度
	struct Vec2 vel_;

	// フレーム
	struct Frame frame_;

	// 不透明度
	int alpha_;

	// 画像
	int gh_;
};