#pragma once
#include "./Class/Texture/Texture.h"

class TextGameOver : public Texture
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextGameOver();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextGameOver();


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