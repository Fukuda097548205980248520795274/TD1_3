#pragma once
#include "./Class/Texture/Texture.h"

class Transition : public Texture
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Transition();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Transition();


	/// <summary>
	/// 動かす
	/// </summary>
	/// <param name="gameFrame">ゲームフレーム</param>
	void Move(int gameFrame) override;

	/// <summary>
	/// 描画する
	/// </summary>
	/// <param name="gameFrame">ゲームフレーム</param>
	void Draw(int gameFrame) override;

};