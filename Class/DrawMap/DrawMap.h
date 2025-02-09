#pragma once
#include <Novice.h>
#include <stdio.h>
#include "Constant.h"
#include "Enumeration.h"

class DrawMap
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
	static void Draw();

	// マップ
	static int map_[kMapRow][kMapColumn];
};