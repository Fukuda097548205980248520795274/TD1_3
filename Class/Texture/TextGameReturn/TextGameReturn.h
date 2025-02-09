#pragma once
#include "./Class/Texture/Texture.h"

class TextGameReturn : public Texture
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextGameReturn();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextGameReturn();


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