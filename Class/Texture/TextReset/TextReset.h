#pragma once
#include "./Class/Texture/Texture.h"

class TextReset : public Texture
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextReset();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextReset();


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
