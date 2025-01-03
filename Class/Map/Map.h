#pragma once
#include <Novice.h>
#include <stdio.h>
#include "Constant.h"
#include "Enumeration.h"

class Map
{
public:

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	/// <param name="fName">ファイルのパス</param>
	static void LoadFile(const char* fName);

	/// <summary>
	/// 描画する
	/// </summary>
	/// <param name="gh">画像</param>
	static void Draw(int gh);

	// マップ
	static TILE map_[kMapRow][kMapColumn];
};