#pragma once
#include "./Class/Texture/Texture.h"

class TextTreasure : public Texture
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextTreasure();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextTreasure();

	/// <summary>
	/// 描画する
	/// </summary>
	/// <param name="gameFrame">ゲームフレーム</param>
	void Draw(int gameFrame) override;
};

