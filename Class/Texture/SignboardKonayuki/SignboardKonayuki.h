#pragma once
#include "./Class/Texture/Texture.h"

class SignboardKonayuki : public Texture
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SignboardKonayuki();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SignboardKonayuki();


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